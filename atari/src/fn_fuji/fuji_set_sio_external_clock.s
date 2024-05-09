        .export         _fuji_set_sio_external_clock

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_set_sio_external_clock(uint16_t rate);
;
.proc _fuji_set_sio_external_clock
        axinto  tmp7
        setax   #t_fuji_set_sio_external_clock
        jsr     _copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::daux1

        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_set_sio_external_clock:
        .byte $df, 0, 0, 0, $ff, $ff
