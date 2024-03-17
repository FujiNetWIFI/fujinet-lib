        .export         _fn_fuji_mount_host_slot

        .import         _bus
        .import         _fn_fuji_success
        .import         copy_fuji_cmd_data
        .import         fn_fuji_hostslots
        .import         popa

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fn_fuji_mount_host_slot(uint8_t slot_num)
;
.proc _fn_fuji_mount_host_slot
        sta     tmp8

        setax   #t_io_mount_host_slot
        jsr     copy_fuji_cmd_data

        mva     tmp8, IO_DCB::daux1
        jsr     _bus
        jmp     _fn_fuji_success

.endproc

.rodata
t_io_mount_host_slot:
        .byte $f9, $00, $00, $00, $ff, $00
