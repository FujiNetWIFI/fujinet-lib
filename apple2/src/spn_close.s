        .export     _spn_close

        .import     _spn_error
        .import     popa
        .import     pusha
        .import     spn_setup

        .include    "sp.inc"
        .include    "macros.inc"

; int8_t _spn_close(uint8_t dest)
;
; Close smartport device
; this changes _spn_payload
; returns any error code from dispatch call
.proc _spn_close
        sta     _spn_cmdlist+1          ; dest
        lda     #SP_CLOSE_PARAM_COUNT
        sta     _spn_cmdlist

        pusha   #SP_CMD_CLOSE
        setax   #_spn_cmdlist
        jsr     dispatch

        sta     _spn_error

        ldx     #$00
        lda     _spn_error
        rts
.endproc
