        .export         _fn_io_put_host_slots
        .import         fn_io_copy_cmd_data, _fn_io_do_bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-io.inc"
        .include        "fn_data.inc"

; void _fn_io_put_host_slots(HostSlot *h)
.proc _fn_io_put_host_slots
        axinto  tmp7
        setax   #t_io_put_host_slots
        jsr     fn_io_copy_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jmp     _fn_io_do_bus
.endproc

.rodata
.define HS8zL .lobyte(.sizeof(HostSlot)*8)
.define HS8zH .hibyte(.sizeof(HostSlot)*8)

t_io_put_host_slots:
        .byte $f3, $80, HS8zL, HS8zH, $00, $00
