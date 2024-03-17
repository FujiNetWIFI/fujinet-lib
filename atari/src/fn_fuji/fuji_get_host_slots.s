        .export         _fuji_get_host_slots

        .import         _bus
        .import         _fuji_success
        .import         copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-fuji.inc"
        .include        "device.inc"

; bool fuji_get_host_slots(struct HostSlot *host_slots)
.proc _fuji_get_host_slots
        axinto  tmp7
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
