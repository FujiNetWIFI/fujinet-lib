        .export         _fuji_qrcode_input
        .export         _fuji_qrcode_output

        .import         io_common

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; Both commands take the byte count as their only parameter, and io_common
; already copies tmp7/8 into DBYT and DAUX -- which is exactly the shape these
; need, same as the base64 input/output pair.

; bool fuji_qrcode_input(char *s, uint16_t len);
;
_fuji_qrcode_input:
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fuji_qrcode_input
        jmp     io_common

; bool fuji_qrcode_output(char *s, uint16_t len);
;
_fuji_qrcode_output:
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fuji_qrcode_output
        jmp     io_common


.rodata
; DCOMND, DSTATS, DBYTLO, DBYTHI, DAUX1, DAUX2
; $ff entries are overwritten by io_common from tmp7/8.
t_fuji_qrcode_input:
        .byte $bc, $80, $ff, $ff, $ff, $ff

t_fuji_qrcode_output:
        .byte $bf, $40, $ff, $ff, $ff, $ff
