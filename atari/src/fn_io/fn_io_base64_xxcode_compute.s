        .export         _fn_io_base64_decode_compute
        .export         _fn_io_base64_encode_compute

        .import         fn_io_copy_cmd_data
        .import         _fn_io_do_bus
        .import         _fn_io_error
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fn_data.inc"

; uint8_t fn_io_base64_decode_compute();
;
_fn_io_base64_decode_compute:
        setax   #t_fn_io_base64_decode_compute

c_common:
        jsr     fn_io_copy_cmd_data
        mva     #$03, IO_DCB::dtimlo
        jsr     _fn_io_do_bus
        jmp     _fn_io_error

; uint8_t fn_io_base64_encode_compute();
;
_fn_io_base64_encode_compute:
        setax   #t_fn_io_base64_encode_compute
        jmp     c_common

.rodata
t_fn_io_base64_decode_compute:
        .byte $cb, $00, $00, $00, $00, $00

t_fn_io_base64_encode_compute:
        .byte $cf, $00, $00, $00, $00, $00
