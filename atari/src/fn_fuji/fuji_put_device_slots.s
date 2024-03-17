        .export         _fuji_put_device_slots

        .import         _bus
        .import         _fuji_success
        .import         copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"
        .include        "fujinet-fuji.inc"

; bool _fuji_put_device_slots(DeviceSlot *device_slots)
.proc _fuji_put_device_slots
        axinto  tmp7

        setax   #t_fuji_put_device_slots
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
.define DS8zL .lobyte(.sizeof(DeviceSlot)*8)
.define DS8zH .hibyte(.sizeof(DeviceSlot)*8)

t_fuji_put_device_slots:
        .byte $f1, $80, DS8zL, DS8zH, $00, $00
