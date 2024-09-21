        .export         _sp_close

        .import         _sp_cmdlist
        .import         sp_dispatch

        .include        "sp.inc"

; int8_t sp_close(uint8_t dest) {
;         sp_cmdlist[1] = dest;
;         sp_cmdlist[0] = SP_CLOSE_PARAM_COUNT;
;         return sp_dispatch(SP_CMD_CLOSE);
; }

.proc _sp_close
        sta     _sp_cmdlist+1                   ; sp_cmdlist[1] = dest;
        lda     #SP_CLOSE_PARAM_COUNT
        sta     _sp_cmdlist                     ; sp_cmdlist[0] = SP_CLOSE_PARAM_COUNT;
        ldx     #SP_CMD_CLOSE
        jmp     sp_dispatch                    ; return sp_dispatch(SP_CMD_CLOSE);
.endproc