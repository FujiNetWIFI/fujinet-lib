        .export     _spn_control

        .import     _spn_count
        .import     _spn_error
        .import     popa
        .import     pusha
        .import     spn_setup

        .include    "sp.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; int8_t _spn_control(uint8_t dest, uint8_t ctrlcode)
;
; Issues a control to the specified device.
; this changes _spn_payload, and _spn_count
; returns any error code from dispatch call
.proc _spn_control
        sta     tmp1                    ; store ctrlcode until we need it
        popa    tmp2                    ; store dest, popa trashes y, so need to store it now instead of later

        pusha   #SP_CMD_CONTROL
        lda     #SP_CONTROL_PARAM_COUNT

        jsr     spn_setup

        sta     _spn_error

        ldx     #$00
        lda     _spn_error              ; forces Z flag based on error value
        rts
.endproc
