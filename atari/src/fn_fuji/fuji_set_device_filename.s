        .export         _fuji_set_device_filename

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         popa

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"


; bool _fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
.proc _fuji_set_device_filename
        axinto  tmp7    ; buffer

        setax   #t_fuji_set_device_filename
        jsr     _copy_fuji_cmd_data

        jsr     popa            ; device slot
        sta     IO_DCB::daux1

        jsr     popa    ; host slot
        beq     :+
        asl     a
        asl     a
        asl     a
        asl     a       ; * 16

:       sta     tmp10

        ; setup aux2 = host_slot * 16 + mode
        jsr     popa    ; mode
        adw1    tmp10, a
        sta     IO_DCB::daux2

        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_set_device_filename:
        .byte $e2, $80, $00, $01, $ff, $00
