        .export         _fn_io_hash_length

        .import         _fn_io_do_bus
        .import         _fn_io_error
        .import         fn_io_copy_cmd_data
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; uint8_t fn_io_hash_length(uint8_t type);
;
.proc _fn_io_hash_length
        sta     tmp7                            ; hash type, one of HashType. No validation done though
        setax   #t_fn_io_hash_length
        jsr    fn_io_copy_cmd_data

        mva     tmp7, IO_DCB::daux1
        mva     #$03, IO_DCB::dtimlo

        jsr     _fn_io_do_bus
        jmp     _fn_io_error
.endproc

.rodata
t_fn_io_hash_length:
        .byte $c6, $40, 1, 0, $ff, 0
