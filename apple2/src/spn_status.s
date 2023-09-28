        .export     _spn_status

        .import     _spn_cmdlist
        .import     _spn_count
        .import     _spn_error
        .import     _spn_payload
        .import     dispatch
        .import     popa
        .import     pusha

        .include    "sp.inc"
        .include    "macros.inc"

; int8_t _spn_status(uint8_t dest, uint8_t statcode)
;
; call smart port for status using given code for given destination
; this changes _spn_payload, and _spn_count
; returns any error code from dispatch call
.proc _spn_status
        sta     _spn_cmdlist+4          ; statcode

        lda     #SP_CONTROL_PARAM_COUNT
        sta     _spn_cmdlist
        jsr     popa                    ; dest
        sta     _spn_cmdlist+1
        lda     #<_spn_payload
        sta     _spn_cmdlist+2
        lda     #>_spn_payload
        sta     _spn_cmdlist+3

        pusha   #SP_CMD_STATUS
        setax   #_spn_cmdlist
        jsr     dispatch

        ; X/Y contains count (of bytes transferred), A contains error
        stx     _spn_count
        sty     _spn_count+1
        sta     _spn_error

        ldx     #$00
        lda     _spn_error              ; forces Z flag based on error value
        rts
.endproc
