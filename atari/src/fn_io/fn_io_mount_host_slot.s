        .export         _fn_io_mount_host_slot
        .import         fn_io_copy_cmd_data, fn_io_hostslots, _fn_io_do_bus
        .import         popa

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_io_mount_host_slot(uint8_t slot_num)
;
.proc _fn_io_mount_host_slot
        sta     tmp8

        setax   #t_io_mount_host_slot
        jsr     fn_io_copy_cmd_data

        mva     tmp8, IO_DCB::daux1
        jmp     _fn_io_do_bus

.endproc

.rodata
t_io_mount_host_slot:
        .byte $f9, $00, $00, $00, $ff, $00
