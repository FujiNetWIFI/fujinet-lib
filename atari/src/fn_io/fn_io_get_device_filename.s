        .export         _fn_io_get_device_filename
        .import         copy_io_cmd_data, _bus, popa
        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_io_get_device_filename(uint8_t device_slot, char *buffer)
.proc _fn_io_get_device_filename
        axinto  tmp7            ; save the buffer pointer
        setax   #t_io_get_device_filename
        jsr     copy_io_cmd_data

        jsr     popa            ; device_slot
        sta     IO_DCB::daux1
        mwa     tmp7, IO_DCB::dbuflo
        jmp     _bus

.endproc

.rodata
t_io_get_device_filename:
        .byte $da, $40, $00, $01, $ff, $00

