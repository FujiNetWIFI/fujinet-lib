        .export         _sp_open_nw

        .import         _sp_cmdlist
        .import         sp_dispatch
        .import         _sp_nw_unit

        .include        "sp.inc"

; int8_t sp_open_nw(uint8_t dest) {
;         sp_cmdlist[0] = SP_OPEN_PARAM_COUNT_NW;
;         sp_cmdlist[1] = dest;
;         sp_cmdlist[2] = sp_nw_unit;
;         return sp_dispatch(SP_CMD_OPEN);
; }

; I don't think anything actually calls sp_open (or nw version) as they are not needed for network device
.proc _sp_open_nw
        sta     _sp_cmdlist+1                   ; sp_cmdlist[1] = dest;
        lda     #SP_OPEN_PARAM_COUNT_NW
        sta     _sp_cmdlist                     ; sp_cmdlist[0] = SP_OPEN_PARAM_COUNT_NW;
        lda     _sp_nw_unit
        sta     _sp_cmdlist+2                   ; sp_cmdlist[2] = sp_nw_unit;
        ldx     #SP_CMD_OPEN
        jmp     sp_dispatch                    ; return sp_dispatch(SP_CMD_OPEN);
.endproc