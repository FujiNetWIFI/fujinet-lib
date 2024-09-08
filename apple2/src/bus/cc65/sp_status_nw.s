        .export         _sp_status_nw

        .import         _sp_cmdlist
        .import         _sp_dispatch
        .import         _sp_nw_unit

        .import         sp_common_params

        .include        "sp.inc"

; int8_t sp_status_nw(uint8_t dest, uint8_t statcode) {
;         uintptr_t payload_address;

;         sp_cmdlist[4] = statcode;

;         sp_cmdlist[0] = SP_STATUS_PARAM_COUNT_NW;
;         sp_cmdlist[1] = dest;

;         payload_address = (uintptr_t)(&sp_payload[0]);
;         sp_cmdlist[2] = payload_address & 0xFF;
;         sp_cmdlist[3] = (payload_address >> 8) & 0xFF;

;         // add the Network unit
;         sp_cmdlist[5] = sp_nw_unit;

;         return sp_dispatch(SP_CMD_STATUS);
; }

.proc _sp_status_nw
        jsr     sp_common_params                ; sp_cmdlist[1..4]

        lda     #SP_STATUS_PARAM_COUNT_NW
        sta     _sp_cmdlist                     ; sp_cmdlist[0] = SP_STATUS_PARAM_COUNT_NW
        lda     _sp_nw_unit
        sta     _sp_cmdlist+5                   ; sp_cmdlist[5] = sp_nw_unit;

        lda     #SP_CMD_STATUS
        jmp     _sp_dispatch                    ; return sp_dispatch(SP_CMD_STATUS);

.endproc