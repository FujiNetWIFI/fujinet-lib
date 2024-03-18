        .export         _fuji_get_host_slots

        .import         _bus
        .import         _fuji_success
        .import         copy_fuji_cmd_data
        .import         popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-fuji.inc"
        .include        "device.inc"

; bool fuji_get_host_slots(struct HostSlot *host_slots, size_t size)
.proc _fuji_get_host_slots
        ; we can ignore the size param, it's more for the C world as a check. For now, leaving atari as hardcoded to 8
        ; axinto  ptr1                    ; size
        popax   tmp7                    ; host_slots
        setax   #t_fuji_get_host_slots
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success

.endproc

.rodata
.define HS8zL .lobyte(.sizeof(HostSlot)*8)
.define HS8zH .hibyte(.sizeof(HostSlot)*8)

t_fuji_get_host_slots:
        .byte $f4, $40, HS8zL, HS8zH, $00, $00
