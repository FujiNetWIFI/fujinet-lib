        .export     _network_open

        .import     _fn_device_error
        .import     _memcpy
        .import     _sp_control
        .import     _sp_find_network
        .import     _sp_init
        .import     _sp_open
        .import     _sp_network
        .import     _sp_payload
        .import     _strlen
        .import     fn_open_mode
        .import     incsp3
        .import     popa
        .import     popax
        .import     pusha
        .import     pushax
        .import     return1

        .include    "sp.inc"
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
        beq     init_error      ; didn't find a FN device at all
        jsr     _sp_find_network
        beq     init_error      ; didn't find a network device on the smartport
        bmi     sp_error        ; actual device error, but negative so we can distinguish between device count and an error

        sta     tmp2            ; store the unit
        sta     _sp_network     ; keep track of network unit for other functions
        jsr     _sp_open
        bne     remove_params_return_error      ; failed to call open, SmartPort error in A (not negative)

        popa    _sp_payload+2   ; mode

        ; save mode
        sta     fn_open_mode

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

        ; add 0x00 to the end of string as a terminator
        mwa     #_sp_payload+4, ptr1
        adw     ptr1, tmp9      ; ptr1 = _sp_payload + string length + 4. i.e. location of 0x9b
        ldy     #$00
        tya
        sta     (ptr1), y

        pusha   tmp2            ; unit
        lda     #'O'            ; open
        jmp     _sp_control     ; do control, and return its errors directly
        ; implicit rts

init_error:
        ; set io error
        lda     #SP_ERR_IO_ERROR
        ; fall through

remove_params_return_error:
        sta     _fn_device_error
        ; need to move SP on 3 bytes to skip unread args
        jsr     incsp3
        jmp     return1         ; FN_ERR_IO_ERROR

sp_error:
        ; smart port error (negative value in a)
        eor     #$ff
        clc
        adc     #$01
        ; never 0, as we detected the error before calling sp_error
        bne     remove_params_return_error

.endproc
