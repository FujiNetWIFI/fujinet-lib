        .export         _fuji_get_directory_position

        .import         _bus
        .import         _fuji_success
        .import         copy_fuji_cmd_data
        .import         popa
        .import         popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_get_directory_position(uint16_t *pos);
;
.proc _fuji_get_directory_position
        axinto  tmp7            ; pos pointer

        setax   #t_fuji_get_directory_position
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_get_directory_position:
        .byte $e5, $40, 2, 0, 0, 0
