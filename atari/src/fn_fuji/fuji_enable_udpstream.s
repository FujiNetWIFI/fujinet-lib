        .export         _fuji_enable_udpstream

        .import         copy_fuji_cmd_data
        .import         _bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fuji_enable_udpstream(uint16_t port, char *host);
;
.proc _fuji_enable_udpstream
        axinto  tmp7                    ; host
        setax   #t_fuji_enable_udpstream
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     popax                   ; port
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2

        jmp     _bus
.endproc

.rodata
t_fuji_enable_udpstream:
        .byte $f0, $80, 64, 0, $ff, $ff
