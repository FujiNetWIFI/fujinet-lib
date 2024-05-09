        .export         _fuji_put_host_slots

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-fuji.inc"
        .include        "device.inc"

; bool _fuji_put_host_slots(HostSlot *h, size_t size)
.proc _fuji_put_host_slots
        ; ignore size param for now, it's always 8 for atari and hardcoded into the size below, but would need to process it if we change the slot count.
        popax  tmp7     ; HostSlot address
        setax   #t_fuji_put_host_slots
        jsr     _copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
.define HS8zL .lobyte(.sizeof(HostSlot)*8)
.define HS8zH .hibyte(.sizeof(HostSlot)*8)

t_fuji_put_host_slots:
        .byte $f3, $80, HS8zL, HS8zH, $00, $00
