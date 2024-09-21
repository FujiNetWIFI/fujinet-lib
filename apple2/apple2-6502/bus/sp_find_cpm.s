        .export         _sp_get_cpm_id
        .export         _sp_cpm_id

        .import         sp_find_device_type

; uint8_t sp_get_cpm_id()
_sp_get_cpm_id:
        lda     #$12
        ldx     #<_sp_cpm_id
        ldy     #>_sp_cpm_id
        jmp     sp_find_device_type

.data
_sp_cpm_id:    .byte $00
