        .export         _fuji_hash_input

        .import         io_common
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"

; bool fuji_hash_input(char *s, uint16_t len);
;
.proc _fuji_hash_input
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fuji_hash_input
        jmp     io_common
.endproc

.rodata
t_fuji_hash_input:
        .byte $c8, $80, $ff, $ff, $ff, $ff
