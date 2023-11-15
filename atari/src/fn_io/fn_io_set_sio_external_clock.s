        .export         _fn_io_set_sio_external_clock

        .import         copy_io_cmd_data
        .import         _bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_io_set_sio_external_clock(uint16_t rate);
;
.proc _fn_io_set_sio_external_clock
        axinto  tmp7
        setax   #t_fn_io_set_sio_external_clock
        jsr     copy_io_cmd_data

        mwa     tmp7, IO_DCB::daux1

        jmp     _bus
.endproc

.rodata
t_fn_io_set_sio_external_clock:
        .byte $df, 0, 0, 0, $ff, $ff
