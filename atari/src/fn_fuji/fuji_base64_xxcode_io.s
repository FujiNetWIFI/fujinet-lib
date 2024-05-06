        .export         _fuji_base64_decode_input
        .export         _fuji_base64_decode_output
        .export         _fuji_base64_encode_input
        .export         _fuji_base64_encode_output

        .import         _bus
        .import         io_common
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_base64_decode_input(char *s, uint16_t len);
;
_fuji_base64_decode_input:
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fuji_base64_decode_input
        jmp     io_common

; bool fuji_base64_decode_output(char *s, uint16_t len);
;
_fuji_base64_decode_output:
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fuji_base64_decode_output
        jmp     io_common

; bool fuji_base64_encode_input(char *s, uint16_t len);
;
_fuji_base64_encode_input:
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fuji_base64_encode_input
        jmp     io_common

; bool fuji_base64_encode_output(char *s, uint16_t len);
;
_fuji_base64_encode_output:
        axinto  tmp7                    ; len in tmp7/8
        setax   #t_fuji_base64_encode_output
        jmp     io_common


.rodata
t_fuji_base64_decode_input:
        .byte $cc, $80, $ff, $ff, $ff, $ff

t_fuji_base64_decode_output:
        .byte $c9, $40, $ff, $ff, $ff, $ff

t_fuji_base64_encode_input:
        .byte $d0, $80, $ff, $ff, $ff, $ff

t_fuji_base64_encode_output:
        .byte $cd, $40, $ff, $ff, $ff, $ff

