        .export         _fn_io_get_hsio_index

        .import         copy_io_cmd_data
        .import         _bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; uint8_t fn_io_get_hsio_index();
;
.proc _fn_io_get_hsio_index
        setax   #t_fn_io_get_hsio_index
        jsr    copy_io_cmd_data

        ; give tmp7/8 as the location of the address to store HSIO index into
        mwa     #tmp7, IO_DCB::dbuflo
        jsr     _bus
        ldx     #$00
        lda     tmp7
        rts
.endproc

.rodata
t_fn_io_get_hsio_index:
        .byte $3f, $40, 1, 0, 0, 0
