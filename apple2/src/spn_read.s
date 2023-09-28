        .export     _spn_read

        .import     _spn_cmdlist
        .import     _spn_error
        .import     _spn_payload
        .import     dispatch
        .import     popa
        .import     pusha

        .include    "sp.inc"
        .include    "macros.inc"

; int8_t _spn_read(uint8_t dest, uint16_t len)
;
; Close smartport device
; this changes _spn_payload
; returns any error code from dispatch call
.proc _spn_read
        sta     _spn_cmdlist+4          ; len (low)
        stx     _spn_cmdlist+5          ; len (high)
        jsr     popa
        sta     _spn_cmdlist+1          ; dest
        lda     #SP_READ_PARAM_COUNT
        sta     _spn_cmdlist
        lda     #<_spn_payload
        sta     _spn_cmdlist+2
        lda     #>_spn_payload
        sta     _spn_cmdlist+3

        pusha   #SP_CMD_READ
        setax   #_spn_cmdlist
        jsr     dispatch

        sta     _spn_error

        ldx     #$00
        lda     _spn_error
        rts
.endproc
