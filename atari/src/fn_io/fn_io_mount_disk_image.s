        .export         _fn_io_mount_disk_image
        .import         fn_io_copy_cmd_data, popa, _fn_io_do_bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_io_mount_disk_image(uint8_t device_slot, uint8_t mode)
.proc _fn_io_mount_disk_image
        sta     tmp8    ; save mode

        setax   #t_io_mount_disk_image
        jsr     fn_io_copy_cmd_data

        mva     tmp8, IO_DCB::daux2

        jsr     popa    ; slot
        sta     IO_DCB::daux1
        mva     #$fe, IO_DCB::dtimlo
        jmp     _fn_io_do_bus
.endproc

.rodata
t_io_mount_disk_image:
        .byte $f8, $00, $00, $00, $ff, $ff