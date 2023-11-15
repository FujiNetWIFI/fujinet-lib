        .export     _fn_io_unmount_host_slot
        .import     fn_io_copy_cmd_data, _fn_io_do_bus

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; void _fn_io_unmount_host_slot(uint8_t hs)
.proc _fn_io_unmount_host_slot
        sta     tmp8    ; save host slot

        setax   #t_io_unmount_host_slot
        jsr     fn_io_copy_cmd_data

        mva     tmp8, IO_DCB::daux1
        jmp     _fn_io_do_bus
.endproc

.rodata
t_io_unmount_host_slot:
        .byte $e6, $00, $00, $00, $ff, $00