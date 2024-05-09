        .export         _fuji_get_hsio_index

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         popa
        .import         popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_get_hsio_index(uint8_t *index);
;
.proc _fuji_get_hsio_index
        axinto  tmp7
        setax   #t_fuji_get_hsio_index
        jsr     _copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_get_hsio_index:
        .byte $3f, $40, 1, 0, 0, 0
