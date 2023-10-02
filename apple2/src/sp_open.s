        .export     _sp_open

        .import     _sp_cmdlist
        .import     _sp_error
        .import     _sp_dispatch
        .import     popa
        .import     pusha

        .include    "sp.inc"
        .include    "macros.inc"

; int8_t _sp_open(uint8_t dest)
;
; Open smartport device
; this changes _sp_payload
; returns any error code from _sp_dispatch call
.proc _sp_open
        sta     _sp_cmdlist+1          ; dest
        lda     #SP_OPEN_PARAM_COUNT
        sta     _sp_cmdlist

        pusha   #SP_CMD_OPEN
        setax   #_sp_cmdlist
        jsr     _sp_dispatch

        sta     _sp_error

        ldx     #$00
        lda     _sp_error
        rts
.endproc
