        .export     _network_http_set_channel_mode
        .export     t_m_ds_l, t_m_ds_h, t_m_mode, t_m_return

        .import     popax

        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_http_set_channel_mode(char* devicespec, uint8_t mode);
_network_http_set_channel_mode:
        ldy     t_round

        ; save the parameters to the function so we can test they were set correctly
        sta     t_m_mode, y

        jsr     popax
        ldy     t_round
        sta     t_m_ds_l, y
        txa
        sta     t_m_ds_h, y


        inc     t_round
        ldx     #$00
        lda     t_m_return, y
        rts

t_round:        .byte 0

t_m_ds_l:       .res 2
t_m_ds_h:       .res 2
t_m_mode:       .res 2
t_m_return:     .res 2
