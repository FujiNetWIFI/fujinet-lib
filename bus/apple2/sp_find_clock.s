        .export         _sp_get_clock_id
        .export         _sp_clock_id

        .import         sp_find_device_type

; uint8_t sp_get_clock_id()
_sp_get_clock_id:
        lda     #$13
        ldx     #<_sp_clock_id
        ldy     #>_sp_clock_id
        jmp     sp_find_device_type

.data
_sp_clock_id:    .byte $00
