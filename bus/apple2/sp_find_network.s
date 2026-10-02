        .export         _sp_get_network_id
        .export         _sp_network

        .import         sp_find_device_type

; uint8_t sp_get_network_id()
_sp_get_network_id:
        lda     #$11
        ldx     #<_sp_network
        ldy     #>_sp_network
        jmp     sp_find_device_type

.data
_sp_network:    .byte $00
