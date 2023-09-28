        .export     _spn_control

        .import     _spn_error
        .import     popa
        .import     pusha

        .include    "sp.inc"
        .include    "macros.inc"

; int8_t _spn_control(uint8_t dest, uint8_t ctrlcode)
;
; Issues a control to the specified device.
; this changes _spn_payload
; returns any error code from dispatch call
.proc _spn_control
        sta     _spn_cmdlist+4          ; ctrlcode

        lda     #SP_STATUS_PARAM_COUNT
        sta     _spn_cmdlist
        jsr     popa                    ; dest
        sta     _spn_cmdlist+1
        lda     #<_spn_payload
        sta     _spn_cmdlist+2
        lda     #>_spn_payload
        sta     _spn_cmdlist+3

        pusha   #SP_CMD_CONTROL
        setax   #_spn_cmdlist
        jsr     dispatch

        sta     _spn_error

        ldx     #$00
        lda     _spn_error              ; forces Z flag based on error value
        rts
.endproc
