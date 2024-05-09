        .export     _fuji_close_directory

        .import     _bus
        .import     _fuji_success
        .import     _copy_fuji_cmd_data

        .include    "macros.inc"

; bool fuji_close_directory(void)
.proc _fuji_close_directory
        setax   #t_fuji_close_directory
        jsr     _copy_fuji_cmd_data
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_close_directory:
        .byte $f5, $00, $00, $00, $00, $00
