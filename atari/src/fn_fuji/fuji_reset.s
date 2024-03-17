        .export         _fuji_reset

        .import         _bus
        .import         copy_fuji_cmd_data

        .include        "macros.inc"

; void  _fuji_reset()
; resets FN. Up to the caller to pause afterwards
.proc _fuji_reset
        setax   #t_io_reset
        jsr     copy_fuji_cmd_data
        jmp     _bus
.endproc

.rodata

t_io_reset:
        .byte $ff, $40, $00, $00, $00, $00
