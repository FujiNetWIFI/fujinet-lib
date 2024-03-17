        .export         _fuji_mount_all

        .import         _bus
        .import         copy_fuji_cmd_data
        .import         return0
        .import         return1

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_mount_all(void)
; returns true for success
;
.proc _fuji_mount_all
        setax   #t_io_mount_all
        jsr     copy_fuji_cmd_data
        jsr     _bus

        lda     IO_DCB::dstats
        cmp     #$01
        beq     ok

        jmp     return0
ok:
        jmp     return1

.endproc

.rodata

t_io_mount_all:
        .byte $d7, $00, $00, $00, $00, $00
