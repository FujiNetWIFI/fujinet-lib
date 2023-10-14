        .export     _sp_write

        .import     _sp_cmdlist
        .import     _sp_error
        .import     _sp_payload
        .import     _sp_dispatch        
        .import     popa
        .import     pusha

        .include    "sp.inc"
        .include    "macros.inc"

; int8_t _sp_write(uint8_t dest, uint16_t len);
;
; Close smartport device
; this changes _sp_payload
; returns any error code from _sp_dispatch call
.proc _sp_write
        sta     _sp_cmdlist+4          ; len (low)
        stx     _sp_cmdlist+5          ; len (high)
        jsr     popa
        sta     _sp_cmdlist+1          ; dest
        lda     #SP_WRITE_PARAM_COUNT
        sta     _sp_cmdlist
        lda     #<_sp_payload
        sta     _sp_cmdlist+2
        lda     #>_sp_payload
        sta     _sp_cmdlist+3

        pusha   #SP_CMD_WRITE
        setax   #_sp_cmdlist
        jsr     _sp_dispatch

        sta     _sp_error

        ldx     #$00
        lda     _sp_error
        rts
.endproc
