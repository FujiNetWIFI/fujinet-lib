        .export         _fuji_hash_compute

        .import         _bus
        .import         _fuji_success
        .import         copy_fuji_cmd_data
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_hash_compute(uint8_t type);
;
.proc _fuji_hash_compute
        sta     tmp7                    ; hash type, one of HashType. No validation done though

        setax   #t_fuji_hash_compute
        jsr    copy_fuji_cmd_data

        mva     tmp7, IO_DCB::daux1
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_hash_compute:
        .byte $c7, $00, $00, $00, $ff, $00
