        .export     _network_write
        .export     t_ds, t_buf, t_len, t_return

        .import     popax

        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len)
_network_write:
        axinto  t_len
        popax   t_buf
        popax   t_ds

        ldx     #$00
        lda     t_return
        rts


t_ds:           .res 2
t_buf:          .res 2
t_len:          .res 2

t_return:       .res 1
