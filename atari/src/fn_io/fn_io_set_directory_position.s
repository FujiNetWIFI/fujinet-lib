        .export     _fn_io_set_directory_position
        .import     fn_io_copy_cmd_data, _fn_io_do_bus

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; void fn_io_set_directory_position(uint16_t pos)
.proc _fn_io_set_directory_position
        axinto  tmp7        ; save the directory pos to tmp7/2

        setax   #t_io_set_directory_position
        jsr     fn_io_copy_cmd_data

        mwa     tmp7, IO_DCB::daux1
        jmp     _fn_io_do_bus
.endproc

.rodata
t_io_set_directory_position:
        .byte $e4, $00, $00, $00, $ff, $ff