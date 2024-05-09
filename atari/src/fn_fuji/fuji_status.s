        .export         _fuji_status

        .import         _copy_fuji_cmd_data
        .import         _bus
        .import         _fuji_success

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_status(FNStatus *status);
;
.proc _fuji_status
        axinto  tmp7                    ; 4 byte buffer location
        setax   #t_fuji_status
        jsr     _copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo

        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_status:
        .byte $53, $40, 4, 0, 0, 0
