        .export         _sp_write

        .import         _sp_cmdlist
        .import         sp_dispatch

        .import         sp_rw_common

        .include        "sp.inc"


.proc _sp_write
        jsr     sp_rw_common                    ; sp_cmdlist[1..5]

        lda     #SP_WRITE_PARAM_COUNT
        sta     _sp_cmdlist                     ; sp_cmdlist[0] = SP_WRITE_PARAM_COUNT

        ldx     #SP_CMD_WRITE
        jmp     sp_dispatch                    ; return sp_dispatch(SP_CMD_WRITE);
.endproc