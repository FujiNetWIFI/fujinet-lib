        .export         _fn_io_status

        .import         copy_io_cmd_data
        .import         _bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_io_status(FNStatus *status);
;
.proc _fn_io_status
        axinto  tmp7                    ; 4 byte buffer location
        setax   #t_fn_io_status
        jsr     copy_io_cmd_data

        mwa     tmp7, IO_DCB::dbuflo

        jmp     _bus
.endproc

.rodata
t_fn_io_status:
        .byte $53, $40, 4, 0, 0, 0
