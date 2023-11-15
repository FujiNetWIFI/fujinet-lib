        .export     _fn_io_set_boot_config
        .import     fn_io_copy_cmd_data, _fn_io_do_bus

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "fn_data.inc"

; void fn_io_set_boot_config(uint8_t toggle)
.proc _fn_io_set_boot_config
        sta     tmp7

        setax   #t_io_set_boot_config
        jsr     fn_io_copy_cmd_data

        mva     tmp7, IO_DCB::daux1
        jmp     _fn_io_do_bus
.endproc

.rodata
t_io_set_boot_config:
        .byte $d9, $00, $00, $00, $ff, $00