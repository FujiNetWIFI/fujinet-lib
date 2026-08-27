        .export         _fuji_qrcode_input
        .export         _fuji_qrcode_encode
        .export         _fuji_qrcode_length
        .export         _fuji_qrcode_output

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         io_common
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_qrcode_input(char *s, uint16_t len);
;
.proc _fuji_qrcode_input
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fuji_qrcode_input
        jmp     io_common
.endproc

; bool fuji_qrcode_encode(uint8_t version, uint8_t ecc, bool shorten);
;
.proc _fuji_qrcode_encode
        asl     a                       ; shorten -> bit 4
        asl     a
        asl     a
        asl     a
        and     #$10
        sta     tmp7

        setax   #t_fuji_qrcode_encode
        jsr     _copy_fuji_cmd_data

        jsr     popa                    ; ecc -> low bits of daux2
        ora     tmp7
        sta     IO_DCB::daux2
        jsr     popa                    ; version -> daux1
        sta     IO_DCB::daux1
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success
.endproc

; bool fuji_qrcode_length(uint8_t output_mode, unsigned long *len);
;
.proc _fuji_qrcode_length
        axinto  tmp7                    ; pointer to len in tmp7/8
        setax   #t_fuji_qrcode_length
        jsr     _copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     popa                    ; output_mode -> daux1
        sta     IO_DCB::daux1
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success
.endproc

; bool fuji_qrcode_output(char *s, uint16_t len);
;
.proc _fuji_qrcode_output
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fuji_qrcode_output
        jmp     io_common
.endproc

.rodata
t_fuji_qrcode_input:
        .byte $bc, $80, $ff, $ff, $ff, $ff

t_fuji_qrcode_encode:
        .byte $bd, $00, $00, $00, $ff, $ff

t_fuji_qrcode_length:
        .byte $be, $40, 4, 0, $ff, 0

t_fuji_qrcode_output:
        .byte $bf, $40, $ff, $ff, $ff, $ff
