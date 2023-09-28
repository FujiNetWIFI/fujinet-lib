        .export     _sp_status

        .import     _sp_cmdlist
        .import     _sp_count
        .import     _sp_error
        .import     _sp_payload
        .import     dispatch
        .import     popa
        .import     pusha

        .include    "sp.inc"
        .include    "macros.inc"

; int8_t _sp_status(uint8_t dest, uint8_t statcode)
;
; call smart port for status using given code for given destination
; this changes _sp_payload, and _sp_count
; returns any error code from dispatch call
.proc _sp_status
        sta     _sp_cmdlist+4          ; statcode

        lda     #SP_CONTROL_PARAM_COUNT
        sta     _sp_cmdlist
        jsr     popa                    ; dest
        sta     _sp_cmdlist+1
        lda     #<_sp_payload
        sta     _sp_cmdlist+2
        lda     #>_sp_payload
        sta     _sp_cmdlist+3

        pusha   #SP_CMD_STATUS
        setax   #_sp_cmdlist
        jsr     dispatch

        ; X/Y contains count (of bytes transferred), A contains error
        stx     _sp_count
        sty     _sp_count+1
        sta     _sp_error

        ldx     #$00
        lda     _sp_error              ; forces Z flag based on error value
        rts
.endproc
