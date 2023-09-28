        .export     _sp_control

        .import     _sp_cmdlist
        .import     _sp_error
        .import     _sp_payload
        .import     dispatch
        .import     popa
        .import     pusha

        .include    "sp.inc"
        .include    "macros.inc"

; int8_t _sp_control(uint8_t dest, uint8_t ctrlcode)
;
; Issues a control to the specified device.
; this changes _sp_payload
; returns any error code from dispatch call
.proc _sp_control
        sta     _sp_cmdlist+4          ; ctrlcode

        lda     #SP_STATUS_PARAM_COUNT
        sta     _sp_cmdlist
        jsr     popa                    ; dest
        sta     _sp_cmdlist+1
        lda     #<_sp_payload
        sta     _sp_cmdlist+2
        lda     #>_sp_payload
        sta     _sp_cmdlist+3

        pusha   #SP_CMD_CONTROL
        setax   #_sp_cmdlist
        jsr     dispatch

        sta     _sp_error

        ldx     #$00
        lda     _sp_error
        rts
.endproc
