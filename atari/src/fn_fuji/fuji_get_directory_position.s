        .export         _fuji_get_directory_position

        .import         copy_fuji_cmd_data
        .import         _bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; uint16_t fuji_get_directory_position();
;
.proc _fuji_get_directory_position
        setax   #t_fuji_get_directory_position
        jsr     copy_fuji_cmd_data

        mwa     #tmp7, IO_DCB::dbuflo
        jsr     _bus
        ldx     tmp8
        lda     tmp7
        rts
.endproc

.rodata
t_fuji_get_directory_position:
        .byte $e5, $40, 2, 0, 0, 0
