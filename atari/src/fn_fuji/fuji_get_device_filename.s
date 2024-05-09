        .export         _fuji_get_device_filename

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         popa

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_get_device_filename(uint8_t device_slot, char *buffer)
.proc _fuji_get_device_filename
        axinto  tmp7            ; save the buffer pointer
        setax   #t_fuji_get_device_filename
        jsr     _copy_fuji_cmd_data

        jsr     popa            ; device_slot
        sta     IO_DCB::daux1
        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success

.endproc

.rodata
t_fuji_get_device_filename:
        .byte $da, $40, $00, $01, $ff, $00

