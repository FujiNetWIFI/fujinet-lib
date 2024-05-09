        .export     _fuji_unmount_host_slot

        .import     _bus
        .import     _fuji_success
        .import     _copy_fuji_cmd_data

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; bool _fuji_unmount_host_slot(uint8_t hs)
.proc _fuji_unmount_host_slot
        sta     tmp8    ; save host slot

        setax   #t_fuji_unmount_host_slot
        jsr     _copy_fuji_cmd_data

        mva     tmp8, IO_DCB::daux1
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_unmount_host_slot:
        .byte $e6, $00, $00, $00, $ff, $00