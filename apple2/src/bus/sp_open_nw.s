        .export         _sp_open_nw

        .import         _sp_cmdlist
        .import         _sp_dispatch
        .import         _sp_nw_unit

        .include        "sp.inc"

; int8_t sp_open_nw(uint8_t dest) {
;         sp_cmdlist[0] = SP_OPEN_PARAM_COUNT_NW;
;         sp_cmdlist[1] = dest;
;         sp_cmdlist[2] = sp_nw_unit;
;         return sp_dispatch(SP_CMD_OPEN);
; }

; shave off 10 bytes doing it in asm
.proc _sp_open_nw
        sta     _sp_cmdlist+1                   ; sp_cmdlist[1] = dest;
        lda     #SP_OPEN_PARAM_COUNT_NW
        sta     _sp_cmdlist                     ; sp_cmdlist[0] = SP_OPEN_PARAM_COUNT_NW;
        lda     _sp_nw_unit
        sta     _sp_cmdlist+2                   ; sp_cmdlist[2] = sp_nw_unit;
        lda     #SP_CMD_OPEN
        jmp     _sp_dispatch                    ; return sp_dispatch(SP_CMD_OPEN);
.endproc