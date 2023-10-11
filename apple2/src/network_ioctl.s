        .export     _network_ioctl

        .import     _fn_device_error
        .import     _sp_clr_pay
        .import     addysp

        .include    "fujinet-network.inc"

; uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, ...);
;
.proc _network_ioctl
        ; not implemented yet

        ; increase SP by Y to clear the params we received, and return error
        jsr     addysp

        jsr     _sp_clr_pay

        ldx     #$00
        stx     _fn_device_error        ; no device error, it's just bad args
        lda     #FN_ERR_BAD_CMD
        rts
.endproc
