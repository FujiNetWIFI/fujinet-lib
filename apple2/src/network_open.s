        .export     _network_open

        .import     _bzero
        .import     _memcpy
        .import     _sp_control
        .import     _sp_find_network
        .import     _sp_init
        .import     _sp_network
        .import     _sp_payload
        .import     _strlen
        .import     incsp3
        .import     popa
        .import     popax
        .import     pusha
        .import     pushax
        .import     return0

        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans);
.proc _network_open
        sta     tmp1            ; trans

        ;; Not sure this is needed, not everything writes 00s to payload buffer, by time it does the final sp_control, it's
        ;; already been trashed with values from init, and find_network.
        ; pushax  #_sp_payload
        ; setax   #$400
        ; jsr     _bzero

        jsr     _sp_init
        jsr     _sp_find_network
        beq     no_network

        sta     tmp2            ; store the unit
        sta     _sp_network     ; keep track of network unit

        popa    _sp_payload+2   ; mode
        popax   ptr1            ; devicespec
        jsr     _strlen

        axinto  tmp9            ; tmp9/10 hold string len
        sta     _sp_payload     ; strlen + 2 for mode and translation
        inc     _sp_payload
        inc     _sp_payload
        lda     #$00
        sta     _sp_payload+1
        lda     tmp1            ; translation
        sta     _sp_payload+3

        ; copy the devicespec to payload[4], and add on 0x9b
        pushax  #_sp_payload+4  ; dst (payload[4])
        pushax  ptr1            ; src (devicespec)
        setax   tmp9            ; strlen from tmp9/10
        jsr     _memcpy         ; copy string. this trashes ptr1-3, hence using tmp9/10

        ; add 0x9b to the end
        mwa     #_sp_payload+4, ptr1
        adw     ptr1, tmp9      ; ptr1 = _sp_payload + string length + 4. i.e. location of 0x9b
        ldy     #$00
        mva     #$9b, {(ptr1), y}

        pusha   tmp2            ; unit
        lda     #'O'            ; open
        jmp     _sp_control

no_network:
        ; need to move SP on 3 bytes to skip unread args
        jsr     incsp3
        jmp     return0
.endproc
