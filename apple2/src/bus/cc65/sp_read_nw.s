        .export         _sp_read_nw

        .import         _sp_cmdlist
        .import         _sp_dispatch
        .import         _sp_nw_unit
        .import         _sp_payload

        .import         popa
        .import         sp_rw_common

        .include        "sp.inc"


; int8_t sp_read_nw(uint8_t dest, uint16_t len) {
;         uintptr_t payload_address;
;
;         sp_cmdlist[4] = len & 0xFF;
;         sp_cmdlist[5] = (len >> 8) & 0xFF;
;         // SP READ can technically take an address, certainly it's sent when doing SP over SLIP, but
;         // in network context it isn't used, so we will stuff the UNIT id of the request into byte 7
;         sp_cmdlist[6] = sp_nw_unit;
;
;         sp_cmdlist[0] = SP_READ_PARAM_COUNT_NW;
;         sp_cmdlist[1] = dest;
;
;         payload_address = (uintptr_t)(&sp_payload[0]);
;         sp_cmdlist[2] = payload_address & 0xFF;
;         sp_cmdlist[3] = (payload_address >> 8) & 0xFF;
;
;         return sp_dispatch(SP_CMD_READ);
; }

; CA65 = 61 bytes, ASM = 38 bytes
.proc _sp_read_nw
        jsr     sp_rw_common                    ; sp_cmdlist[1..5]

        lda     #SP_READ_PARAM_COUNT_NW
        sta     _sp_cmdlist                     ; sp_cmdlist[0] = SP_READ_PARAM_COUNT_NW
        lda     _sp_nw_unit
        sta     _sp_cmdlist+6                   ; sp_cmdlist[6] = sp_nw_unit;

        lda     #SP_CMD_READ
        jmp     _sp_dispatch                    ; return sp_dispatch(SP_CMD_READ);
.endproc