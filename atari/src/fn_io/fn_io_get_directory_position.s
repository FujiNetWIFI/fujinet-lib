        .export         _fn_io_get_directory_position

        .import         fn_io_copy_cmd_data
        .import         _fn_io_do_bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fn_data.inc"

; uint16_t fn_io_get_directory_position();
;
.proc _fn_io_get_directory_position
        setax   #t_fn_io_get_directory_position
        jsr     fn_io_copy_cmd_data

        mwa     #tmp7, IO_DCB::dbuflo
        jsr     _fn_io_do_bus
        ldx     tmp8
        lda     tmp7
        rts
.endproc

.rodata
t_fn_io_get_directory_position:
        .byte $e5, $40, 2, 0, 0, 0
