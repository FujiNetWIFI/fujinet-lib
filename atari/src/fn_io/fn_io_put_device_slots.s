        .export         _fn_io_put_device_slots

        .import         copy_io_cmd_data, _bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"
        .include        "fujinet-io.inc"

; void _fn_io_put_device_slots(DeviceSlot *device_slots)
.proc _fn_io_put_device_slots
        axinto  tmp7

        setax   #t_io_put_device_slots
        jsr     copy_io_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jmp     _bus
.endproc

.rodata
.define DS8zL .lobyte(.sizeof(DeviceSlot)*8)
.define DS8zH .hibyte(.sizeof(DeviceSlot)*8)

t_io_put_device_slots:
        .byte $f1, $80, DS8zL, DS8zH, $00, $00
