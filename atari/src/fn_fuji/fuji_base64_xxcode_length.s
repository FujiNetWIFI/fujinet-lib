        .export         _fuji_base64_decode_length
        .export         _fuji_base64_encode_length

        .import         copy_fuji_cmd_data
        .import         _bus
        .import         _fuji_success
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_base64_decode_length(unsigned long *len);
;
_fuji_base64_decode_length:
        axinto   tmp7                   ; pointer to len in tmp7/8
        setax   #t_fuji_base64_decode_length

de_common:
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success

; bool fuji_base64_encode_length(unsigned long *len);
;
_fuji_base64_encode_length:
        axinto  tmp7                   ; pointer to len in tmp7/8
        setax   #t_fuji_base64_encode_length
        jmp     de_common

.rodata
t_fuji_base64_decode_length:
        .byte $ca, $40, 4, 0, 0, 0

t_fuji_base64_encode_length:
        .byte $ce, $40, 4, 0, 0, 0
