        .export         _fuji_hash_length

        ; .import         _bus
        ; .import         _fuji_success
        ; .import         _copy_fuji_cmd_data
        ; .import         popa, popax
        .import         return0

        ; .include        "zp.inc"
        ; .include        "macros.inc"
        ; .include        "device.inc"

; deprecated!
; bool fuji_hash_length(uint8_t type);
;
.proc _fuji_hash_length
        jmp     return0

        ; sta     tmp7                            ; hash type, one of HashType. No validation done though
        ; setax   #t_fuji_hash_length
        ; jsr    _copy_fuji_cmd_data

        ; mva     tmp7, IO_DCB::daux1
        ; mva     #$03, IO_DCB::dtimlo

        ; jsr     _bus
        ; jmp     _fuji_success
.endproc

; .rodata
; t_fuji_hash_length:
;         .byte $c6, $40, 1, 0, $ff, 0
