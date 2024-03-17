        .export         _fuji_hash_length

        .import         _bus
        .import         _fuji_error
        .import         copy_fuji_cmd_data
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; uint8_t fuji_hash_length(uint8_t type);
;
.proc _fuji_hash_length
        sta     tmp7                            ; hash type, one of HashType. No validation done though
        setax   #t_fuji_hash_length
        jsr    copy_fuji_cmd_data

        mva     tmp7, IO_DCB::daux1
        mva     #$03, IO_DCB::dtimlo

        jsr     _bus
        jmp     _fuji_error
.endproc

.rodata
t_fuji_hash_length:
        .byte $c6, $40, 1, 0, $ff, 0
