        .export     _fuji_set_boot_mode

        .import     _bus
        .import     _fuji_success
        .import     copy_fuji_cmd_data

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; void _fuji_set_boot_mode(uint8_t mode)
.proc _fuji_set_boot_mode
        sta     tmp7    ; save mode

        setax   #t_fuji_set_boot_mode
        jsr     copy_fuji_cmd_data

        mva     tmp7, IO_DCB::daux1
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_set_boot_mode:
        .byte $d6, $00, $00, $00, $ff, $00