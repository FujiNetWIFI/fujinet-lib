        .export         _fuji_get_device_slots
        .export         t_fuji_get_device_slots

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-fuji.inc"
        .include        "device.inc"

; bool fuji_get_device_slots(DeviceSlot *device_slots, size_t size)
;
.proc _fuji_get_device_slots
        ; we can ignore the size param, it's more for the C world as a check. For now, leaving atari as hardcoded to 8
        ; axinto  ptr1                    ; size
        popax   tmp7
        setax   #t_fuji_get_device_slots
        jsr     _copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
.define DS8zL .lobyte(.sizeof(DeviceSlot)*8)
.define DS8zH .hibyte(.sizeof(DeviceSlot)*8)

t_fuji_get_device_slots:
        .byte $f2, $40, DS8zL, DS8zH, $00, $00
