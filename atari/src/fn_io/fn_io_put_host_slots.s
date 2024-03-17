        .export         _fn_io_put_host_slots
        .import         copy_io_cmd_data, _bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-io.inc"
        .include        "device.inc"

; bool _fn_io_put_host_slots(HostSlot *h)
.proc _fn_io_put_host_slots
        axinto  tmp7
        setax   #t_io_put_host_slots
        jsr     copy_io_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fn_io_success
.endproc

.rodata
.define HS8zL .lobyte(.sizeof(HostSlot)*8)
.define HS8zH .hibyte(.sizeof(HostSlot)*8)

t_io_put_host_slots:
        .byte $f3, $80, HS8zL, HS8zH, $00, $00
