        .export     _fuji_set_directory_position

        .import     _bus
        .import     _fuji_success
        .import     copy_fuji_cmd_data

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; bool fuji_set_directory_position(uint16_t pos)
.proc _fuji_set_directory_position
        axinto  tmp7        ; save the directory pos to tmp7/8

        setax   #t_fuji_set_directory_position
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::daux1
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_set_directory_position:
        .byte $e4, $00, $00, $00, $ff, $ff