        .export         _fuji_base64_decode_compute
        .export         _fuji_base64_encode_compute

        .import         _copy_fuji_cmd_data
        .import         _bus
        .import         _fuji_success
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_base64_decode_compute();
;
_fuji_base64_decode_compute:
        setax   #t_fuji_base64_decode_compute

c_common:
        jsr     _copy_fuji_cmd_data
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success

; bool fuji_base64_encode_compute();
;
_fuji_base64_encode_compute:
        setax   #t_fuji_base64_encode_compute
        jmp     c_common

.rodata
t_fuji_base64_decode_compute:
        .byte $cb, $00, $00, $00, $00, $00

t_fuji_base64_encode_compute:
        .byte $cf, $00, $00, $00, $00, $00
