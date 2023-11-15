        .export         _fn_io_status

        .import         fn_io_copy_cmd_data
        .import         _fn_io_do_bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_io_status(FNStatus *status);
;
.proc _fn_io_status
        axinto  tmp7                    ; 4 byte buffer location
        setax   #t_fn_io_status
        jsr     fn_io_copy_cmd_data

        mwa     tmp7, IO_DCB::dbuflo

        jmp     _fn_io_do_bus
.endproc

.rodata
t_fn_io_status:
        .byte $53, $40, 4, 0, 0, 0
