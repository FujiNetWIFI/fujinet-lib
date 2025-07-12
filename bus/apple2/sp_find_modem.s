        .export         _sp_get_modem_id
        .export         _sp_modem_id

        .import         sp_find_device_type

; uint8_t sp_get_modem_id()
_sp_get_modem_id:
        lda     #$15
        ldx     #<_sp_modem_id
        ldy     #>_sp_modem_id
        jmp     sp_find_device_type

.data
_sp_modem_id:    .byte $00
