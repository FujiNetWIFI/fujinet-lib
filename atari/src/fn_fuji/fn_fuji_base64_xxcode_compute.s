        .export         _fn_fuji_base64_decode_compute
        .export         _fn_fuji_base64_encode_compute

        .import         copy_fuji_cmd_data
        .import         _bus
        .import         _fn_fuji_error
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; uint8_t fn_fuji_base64_decode_compute();
;
_fn_fuji_base64_decode_compute:
        setax   #t_fn_fuji_base64_decode_compute

c_common:
        jsr     copy_fuji_cmd_data
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fn_fuji_error

; uint8_t fn_fuji_base64_encode_compute();
;
_fn_fuji_base64_encode_compute:
        setax   #t_fn_fuji_base64_encode_compute
        jmp     c_common

.rodata
t_fn_fuji_base64_decode_compute:
        .byte $cb, $00, $00, $00, $00, $00

t_fn_fuji_base64_encode_compute:
        .byte $cf, $00, $00, $00, $00, $00
