        .export     _network_http_post
        .export     t_ds, t_data, t_return

        .import     popax

        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_http_post(char* devicespec, char *data)
_network_http_post:
        axinto  t_data
        popax   t_ds

        ldx     #$00
        lda     t_return
        rts


t_ds:           .res 2
t_data:         .res 2

t_return:       .res 1
