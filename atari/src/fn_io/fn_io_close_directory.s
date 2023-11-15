        .export     _fn_io_close_directory

        .import     _bus
        .import     copy_io_cmd_data

        .include    "macros.inc"

; void fn_io_close_directory(void)
.proc _fn_io_close_directory
        setax   #t_io_close_directory
        jsr     copy_io_cmd_data
        jmp     _bus
.endproc

.rodata
t_io_close_directory:
        .byte $f5, $00, $00, $00, $00, $00
