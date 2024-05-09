        .export     _fuji_set_boot_config

        .import     _bus
        .import     _fuji_success
        .import     _copy_fuji_cmd_data

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; bool fuji_set_boot_config(uint8_t toggle)
.proc _fuji_set_boot_config
        sta     tmp7

        setax   #t_fuji_set_boot_config
        jsr     _copy_fuji_cmd_data

        mva     tmp7, IO_DCB::daux1
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_set_boot_config:
        .byte $d9, $00, $00, $00, $ff, $00