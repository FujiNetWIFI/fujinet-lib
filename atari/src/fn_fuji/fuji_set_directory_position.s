        .export     _fuji_set_directory_position
        .import     copy_fuji_cmd_data, _bus

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; void fuji_set_directory_position(uint16_t pos)
.proc _fuji_set_directory_position
        axinto  tmp7        ; save the directory pos to tmp7/2

        setax   #t_io_set_directory_position
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::daux1
        jmp     _bus
.endproc

.rodata
t_io_set_directory_position:
        .byte $e4, $00, $00, $00, $ff, $ff