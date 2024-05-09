        .export         _fuji_mount_all

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_mount_all(void)
; returns true for success
;
.proc _fuji_mount_all
        setax   #t_fuji_mount_all
        jsr     _copy_fuji_cmd_data
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata

t_fuji_mount_all:
        .byte $d7, $00, $00, $00, $00, $00
