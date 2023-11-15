        .export         _fn_io_hash_output

        .import         _fn_io_do_bus
        .import         _fn_io_error
        .import         fn_io_copy_cmd_data
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fn_data.inc"

; uint8_t fn_io_hash_output(uint8_t output_type, char *s, uint16_t len);
;
.proc _fn_io_hash_output
        axinto  tmp7                    ; len pointer

        setax   #t_fn_io_hash_output
        jsr     fn_io_copy_cmd_data

        setax   tmp7
        sta     IO_DCB::dbytlo
        stx     IO_DCB::dbythi
        mva     #$03, IO_DCB::dtimlo
        jsr     popax                   ; buffer address (s)
        sta     IO_DCB::dbuflo
        stx     IO_DCB::dbufhi
        jsr     popa                    ; output type (0 = bin, 1 = hex)
        sta     IO_DCB::daux1

        jsr     _fn_io_do_bus
        jmp     _fn_io_error
.endproc

.rodata
t_fn_io_hash_output:
        .byte $c5, $40, $ff, $ff, 1, 0
