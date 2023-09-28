        .export     _spn_open

        .import     _spn_error
        .import     popa
        .import     pusha
        .import     spn_setup

        .include    "sp.inc"
        .include    "macros.inc"

; int8_t _spn_open(uint8_t dest)
;
; Open smartport device
; this changes _spn_payload
; returns any error code from dispatch call
.proc _spn_open
        sta     _spn_cmdlist+1          ; dest
        lda     #SP_OPEN_PARAM_COUNT
        sta     _spn_cmdlist

        pusha   #SP_CMD_OPEN
        setax   #_spn_cmdlist
        jsr     dispatch

        sta     _spn_error

        ldx     #$00
        lda     _spn_error
        rts
.endproc
