        .export     _fn_io_close_directory

        .include    "macros.inc"
        .import     _fn_io_bus

; void fn_io_close_directory(void)
.proc _fn_io_close_directory
        setax   #t_io_close_directory
        jmp     _fn_io_bus
.endproc

.rodata
t_io_close_directory:
        .byte $f5, $00, $00, $00, $00, $00
