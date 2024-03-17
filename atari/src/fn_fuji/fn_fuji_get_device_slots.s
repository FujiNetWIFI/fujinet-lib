        .export         _fn_fuji_get_device_slots, t_io_get_device_slots
        .import         copy_fuji_cmd_data, _bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-fuji.inc"
        .include        "device.inc"

; void fn_fuji_get_device_slots(DeviceSlot *device_slots)
;
.proc _fn_fuji_get_device_slots
        axinto  tmp7
        setax   #t_io_get_device_slots
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jmp     _bus
.endproc

.rodata
.define DS8zL .lobyte(.sizeof(DeviceSlot)*8)
.define DS8zH .hibyte(.sizeof(DeviceSlot)*8)

t_io_get_device_slots:
        .byte $f2, $40, DS8zL, DS8zH, $00, $00
