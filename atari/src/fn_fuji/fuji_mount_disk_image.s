        .export         _fuji_mount_disk_image

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         popa

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_mount_disk_image(uint8_t device_slot, uint8_t mode)
.proc _fuji_mount_disk_image
        sta     tmp8    ; save mode

        setax   #t_fuji_mount_disk_image
        jsr     _copy_fuji_cmd_data

        mva     tmp8, IO_DCB::daux2

        jsr     popa    ; slot
        sta     IO_DCB::daux1
        mva     #$fe, IO_DCB::dtimlo
        jsr     _bus
        jsr     _fuji_success

.endproc

.rodata
t_fuji_mount_disk_image:
        .byte $f8, $00, $00, $00, $ff, $ff