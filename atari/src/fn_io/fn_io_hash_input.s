        .export         _fn_io_hash_input

        .import         io_common
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"

; uint8_t fn_io_hash_input(char *s, uint16_t len);
;
.proc _fn_io_hash_input
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fn_io_hash_input
        jmp     io_common
.endproc

.rodata
t_fn_io_hash_input:
        .byte $c8, $80, $ff, $ff, $ff, $ff
