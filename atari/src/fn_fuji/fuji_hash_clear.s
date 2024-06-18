        .export         _fuji_hash_clear

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"

; bool fuji_hash_clear();
;
.proc _fuji_hash_clear
        setax   #t_fuji_hash_clear
        jsr     _copy_fuji_cmd_data
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_hash_clear:
        .byte $c2, $00, $00, $00, $00, $00
