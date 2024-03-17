        .export     _fuji_close_directory

        .import     _bus
        .import     copy_fuji_cmd_data

        .include    "macros.inc"

; void fuji_close_directory(void)
.proc _fuji_close_directory
        setax   #t_io_close_directory
        jsr     copy_fuji_cmd_data
        jmp     _bus
.endproc

.rodata
t_io_close_directory:
        .byte $f5, $00, $00, $00, $00, $00
