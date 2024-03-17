        .export         _fn_fuji_status

        .import         copy_fuji_cmd_data
        .import         _bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_fuji_status(FNStatus *status);
;
.proc _fn_fuji_status
        axinto  tmp7                    ; 4 byte buffer location
        setax   #t_fn_fuji_status
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo

        jmp     _bus
.endproc

.rodata
t_fn_fuji_status:
        .byte $53, $40, 4, 0, 0, 0
