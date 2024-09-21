        .export         _sp_open

        .import         _sp_cmdlist
        .import         sp_dispatch

        .include        "sp.inc"

; int8_t sp_open(uint8_t dest) {
;         sp_cmdlist[1] = dest;
;         sp_cmdlist[0] = SP_OPEN_PARAM_COUNT;
;         return sp_dispatch(SP_CMD_OPEN);
; }

; I don't think anything actually calls sp_open (or nw version) as they are not needed for network device
.proc _sp_open
        sta     _sp_cmdlist+1                   ; sp_cmdlist[1] = dest;
        lda     #SP_OPEN_PARAM_COUNT
        sta     _sp_cmdlist                     ; sp_cmdlist[0] = SP_OPEN_PARAM_COUNT;
        ldx     #SP_CMD_OPEN
        jmp     sp_dispatch                    ; return sp_dispatch(SP_CMD_OPEN);
.endproc