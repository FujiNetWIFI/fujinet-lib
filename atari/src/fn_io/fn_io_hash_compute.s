        .export         _fn_io_hash_compute

        .import         _fn_io_do_bus
        .import         _fn_io_error
        .import         fn_io_copy_cmd_data
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fn_data.inc"

; uint8_t fn_io_hash_compute(uint8_t type);
;
.proc _fn_io_hash_compute
        sta     tmp7                    ; hash type, one of HashType. No validation done though

        setax   #t_fn_io_hash_compute
        jsr    fn_io_copy_cmd_data

        mva     tmp7, IO_DCB::daux1
        mva     #$03, IO_DCB::dtimlo
        jsr     _fn_io_do_bus
        jmp     _fn_io_error
.endproc

.rodata
t_fn_io_hash_compute:
        .byte $c7, $00, $00, $00, $ff, $00
