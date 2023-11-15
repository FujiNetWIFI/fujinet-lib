        .export         _fn_io_reset

        .import         _bus
        .import         copy_io_cmd_data

        .include        "macros.inc"

; void  _fn_io_reset()
; resets FN. Up to the caller to pause afterwards
.proc _fn_io_reset
        setax   #t_io_reset
        jsr     copy_io_cmd_data
        jmp     _bus
.endproc

.rodata

t_io_reset:
        .byte $ff, $40, $00, $00, $00, $00
