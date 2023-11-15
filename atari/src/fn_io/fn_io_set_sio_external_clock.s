        .export         _fn_io_set_sio_external_clock

        .import         fn_io_copy_cmd_data
        .import         _fn_io_do_bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_io_set_sio_external_clock(uint16_t rate);
;
.proc _fn_io_set_sio_external_clock
        axinto  tmp7
        setax   #t_fn_io_set_sio_external_clock
        jsr     fn_io_copy_cmd_data

        mwa     tmp7, IO_DCB::daux1

        jmp     _fn_io_do_bus
.endproc

.rodata
t_fn_io_set_sio_external_clock:
        .byte $df, 0, 0, 0, $ff, $ff
