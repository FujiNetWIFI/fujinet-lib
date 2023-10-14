        .export     _network_ioctl

        .import     _fn_device_error
        .import     _fn_error
        .import     _memcpy
        .import     _sp_clr_payload
        .import     _sp_control
        .import     _sp_init
        .import     _sp_is_init
        .import     _sp_network
        .import     _sp_payload
        .import     addysp
        .import     incsp2
        .import     popa
        .import     popax
        .import     pusha
        .import     pushax

        .include    "fujinet-network.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, ...);
;
;
; uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, void *buffer);
;
; For apple, aux1/2 = LENGTH of payload, if there is any
; 1 extra WORD arg, the payload buffer to copy into sp_payload[2..] - of length aux1/aux2
; if the arg is NULL (0), just CLEAR payload 2 and don't copy
.proc _network_ioctl
        cpy     #$07                    ; 5 for standard args, 1 extra args at 2 bytes each = 5 + 2 = 7 bytes on stack. if not, we have been called incorrectly
        bne     @args_error

        ; this is one function that may be called outside an open/etc, so check if init is done
        lda     _sp_is_init
        bne     :+
        jsr     _sp_init

:       lda     _sp_network             ; check the network id is not 0
        beq     @args_error

        jsr     _sp_clr_payload         ; nuke the payload

        ldy     #$00
        sty     _fn_device_error        ; no error yet

        popax   ptr1                    ; buffer
        jsr     incsp2                  ; devicespec - unused on apple, _sp_network must be set
        popa    tmp6                    ; aux2, high byte of length
        popa    tmp5                    ; aux1, low byte of length
        popa    tmp4                    ; control command

        lda     ptr1                    ; check if the buffer is set
        ora     ptr1+1
        beq     :+                      ; it's 0, so skip the copy

        ; there is a buffer, so copy it to sp_payload[2]
        pushax  #_sp_payload+2          ; dest
        pushax  ptr1                    ; src
        setax   tmp5                    ; length
        jsr     _memcpy

:       mwa     tmp5, _sp_payload       ; store length in _sp_payload[0,1]

        pusha   _sp_network
        lda     tmp4
        jsr     _sp_control
        jmp     _fn_error               ; process any error from sp_control

@args_error:
        ; increase SP by Y to clear the params we received, and return error
        jsr     addysp
        ldx     #$00
        stx     _fn_device_error        ; no device error, it's just bad args
        lda     #FN_ERR_BAD_CMD
        rts

.endproc
