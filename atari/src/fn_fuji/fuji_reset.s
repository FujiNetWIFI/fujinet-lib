        .export         _fuji_reset

        .import         _bus
        .import         _fuji_success
        .import         copy_fuji_cmd_data

        .include        "macros.inc"

; bool _fuji_reset()
; resets FN. Up to the caller to pause afterwards
.proc _fuji_reset
        setax   #t_fuji_reset
        jsr     copy_fuji_cmd_data
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata

t_fuji_reset:
        .byte $ff, $40, $00, $00, $00, $00
