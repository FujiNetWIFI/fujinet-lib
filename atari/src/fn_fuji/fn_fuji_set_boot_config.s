        .export     _fn_fuji_set_boot_config
        .import     copy_fuji_cmd_data, _bus

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; void fn_fuji_set_boot_config(uint8_t toggle)
.proc _fn_fuji_set_boot_config
        sta     tmp7

        setax   #t_io_set_boot_config
        jsr     copy_fuji_cmd_data

        mva     tmp7, IO_DCB::daux1
        jmp     _bus
.endproc

.rodata
t_io_set_boot_config:
        .byte $d9, $00, $00, $00, $ff, $00