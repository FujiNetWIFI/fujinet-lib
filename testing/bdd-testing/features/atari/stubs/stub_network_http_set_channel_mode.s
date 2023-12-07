        .export     _network_http_set_channel_mode
        .export     t_ds, t_mode, t_return

        .import     popax

        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_http_set_channel_mode(char* devicespec, uint8_t mode);
_network_http_set_channel_mode:
        sta     t_mode
        popax   t_ds

        ldx     #$00
        lda     t_return
        rts


t_ds:           .res 2
t_mode:         .res 1

t_return:       .res 1
