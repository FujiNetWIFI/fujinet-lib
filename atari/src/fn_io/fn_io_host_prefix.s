        .export         _fn_io_get_host_prefix
        .export         _fn_io_set_host_prefix

        .import         fn_io_copy_cmd_data
        .import         _fn_io_do_bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_io_get_host_prefix(uint8_t hs, char *prefix);
;
_fn_io_get_host_prefix:
        axinto  tmp7                    ; string memory location to read into
        setax   #t_fn_io_get_host_prefix

hp_common:
        jsr     fn_io_copy_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     popa                    ; host slot
        sta     IO_DCB::daux1

        jmp     _fn_io_do_bus


; void fn_io_set_host_prefix(uint8_t hs, char *prefix);
;
_fn_io_set_host_prefix:
        axinto  tmp7                    ; string memory location to read into
        setax   #t_fn_io_set_host_prefix
        jmp     hp_common


.rodata
t_fn_io_get_host_prefix:
        .byte $e0, $40, 0, 1, $ff, 0

t_fn_io_set_host_prefix:
        .byte $e1, $80, 0, 1, $ff, 0
