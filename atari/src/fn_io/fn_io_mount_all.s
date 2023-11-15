        .export         _fn_io_mount_all

        .import         _bus
        .import         copy_io_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; uint8_t fn_io_mount_all(void)
;
; 1 = success, otherwise error
.proc _fn_io_mount_all
        setax   #t_io_mount_all
        jsr     copy_io_cmd_data
        jsr     _bus

        ldx     #$00
        lda     IO_DCB::dstats
        rts
.endproc

.rodata

t_io_mount_all:
        .byte $d7, $00, $00, $00, $00, $00
