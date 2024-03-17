        .export     _fuji_open_directory

        .import     _bus
        .import     _fuji_success
        .import     copy_fuji_cmd_data
        .import     popa, return0

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; bool fuji_open_directory(uint8_t host_slot, char *path_filter)
;
; returns the success status, true for ok, false for error
.proc _fuji_open_directory
        axinto  tmp7            ; save location of path+filter buffer

        setax   #t_io_open_directory
        jsr     copy_fuji_cmd_data

        jsr     popa            ; host slot
        sta     IO_DCB::daux1
        mwa     tmp7, IO_DCB::dbuflo

        jsr     _bus
        jmp     _fuji_success

.endproc

.rodata
t_io_open_directory:
        .byte $f7, $80, $00, $01, $ff, $00
