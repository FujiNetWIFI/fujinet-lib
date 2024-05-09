        .export     _fuji_unmount_disk_image

        .import     _bus
        .import     _fuji_success
        .import     _copy_fuji_cmd_data

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; bool _fuji_unmount_disk_image(uint8_t ds)
.proc _fuji_unmount_disk_image
        sta     tmp8    ; save device slot

        setax   #t_fuji_unmount_disk_image
        jsr     _copy_fuji_cmd_data

        mva     tmp8, IO_DCB::daux1
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_unmount_disk_image:
        .byte $e9, $00, $00, $00, $ff, $00