        .export         _fn_io_enable_udpstream

        .import         fn_io_copy_cmd_data
        .import         _fn_io_do_bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fn_data.inc"

; void fn_io_enable_udpstream(uint16_t port, char *host);
;
.proc _fn_io_enable_udpstream
        axinto  tmp7                    ; host
        setax   #t_fn_io_enable_udpstream
        jsr     fn_io_copy_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     popax                   ; port
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2

        jmp     _fn_io_do_bus
.endproc

.rodata
t_fn_io_enable_udpstream:
        .byte $f0, $80, 64, 0, $ff, $ff
