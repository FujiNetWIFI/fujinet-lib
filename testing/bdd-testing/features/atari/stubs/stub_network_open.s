        .export     _network_open
        .export     t_ds, t_mode, t_trans, t_return

        .import     popa
        .import     popax

        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans);
_network_open:
        sta     t_trans
        popa    t_mode
        popax   t_ds

        ldx     #$00
        lda     t_return
        rts


t_ds:           .res 2
t_mode:         .res 1
t_trans:        .res 1

t_return:       .res 1
