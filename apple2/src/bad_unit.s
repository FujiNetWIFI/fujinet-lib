        .export     _bad_unit
        .import     _fn_device_error

        .include    "fujinet-network.inc"
        .include    "sp.inc"

.proc _bad_unit
        lda     #SP_ERR_BAD_UNIT
        sta     _fn_device_error

        ldx     #$00
        lda     #FN_ERR_BAD_CMD
        rts
.endproc