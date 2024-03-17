        .export         _fuji_base64_decode_compute
        .export         _fuji_base64_encode_compute

        .import         copy_fuji_cmd_data
        .import         _bus
        .import         _fuji_error
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; uint8_t fuji_base64_decode_compute();
;
_fuji_base64_decode_compute:
        setax   #t_fuji_base64_decode_compute

c_common:
        jsr     copy_fuji_cmd_data
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_error

; uint8_t fuji_base64_encode_compute();
;
_fuji_base64_encode_compute:
        setax   #t_fuji_base64_encode_compute
        jmp     c_common

.rodata
t_fuji_base64_decode_compute:
        .byte $cb, $00, $00, $00, $00, $00

t_fuji_base64_encode_compute:
        .byte $cf, $00, $00, $00, $00, $00
