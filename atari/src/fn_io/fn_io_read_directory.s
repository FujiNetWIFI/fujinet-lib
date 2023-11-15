        .export     _fn_io_read_directory
        .import     fn_io_copy_cmd_data, popa, _fn_io_do_bus

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "fn_data.inc"

; char *fn_io_read_directory(unsigned char maxlen, unsigned char aux2, char *buffer)
;
; See https://github.com/FujiNetWIFI/fujinet-platformio/wiki/BUS-Command-%24F6-Read-Directory for aux2 value
.proc _fn_io_read_directory
        axinto  tmp7                    ; buffer location

        setax   #t_io_read_directory
        jsr     fn_io_copy_cmd_data

        jsr     popa                    ; aux2 param
        sta     IO_DCB::daux2

        jsr     popa                    ; maxlen param
        sta     IO_DCB::dbytlo
        sta     IO_DCB::daux1

        mwa     tmp7, IO_DCB::dbuflo
        ldy     #$00
        mva     #$7f, {(tmp7), y}       ; it's the thing to do apparantly. I think this is a DIR marker

        jsr     _fn_io_do_bus
        setax   tmp7
        rts

.endproc

.rodata
t_io_read_directory:
        .byte $f6, $40, $ff, $00, $ff, $ff
