        .export     _fn_io_open_directory

        .import     _fn_io_error
        .import     fn_io_copy_cmd_data, _fn_io_do_bus
        .import     popa, return0

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; int fn_io_open_directory(uint8_t host_slot, char *path_filter)
;
; returns the error status, 0 for no error, 1 for error
.proc _fn_io_open_directory
        axinto  tmp7            ; save location of path+filter buffer

        setax   #t_io_open_directory
        jsr     fn_io_copy_cmd_data

        jsr     popa            ; host slot
        sta     IO_DCB::daux1
        mwa     tmp7, IO_DCB::dbuflo

        jsr     _fn_io_do_bus
        jmp     _fn_io_error

.endproc

.rodata
t_io_open_directory:
        .byte $f7, $80, $00, $01, $ff, $00
