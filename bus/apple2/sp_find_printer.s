        .export         _sp_get_printer_id
        .export         _sp_printer_id

        .import         sp_find_device_type

; uint8_t sp_get_printer_id()
_sp_get_printer_id:
        lda     #$14
        ldx     #<_sp_printer_id
        ldy     #>_sp_printer_id
        jmp     sp_find_device_type

.data
_sp_printer_id:    .byte $00
