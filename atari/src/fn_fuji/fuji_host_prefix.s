        .export         _fuji_get_host_prefix
        .export         _fuji_set_host_prefix

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         popa
        .import         popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_get_host_prefix(uint8_t hs, char *prefix);
;
_fuji_get_host_prefix:
        axinto  tmp7                    ; string memory location to read into
        setax   #t_fuji_get_host_prefix

hp_common:
        jsr     _copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     popa                    ; host slot
        sta     IO_DCB::daux1

        jsr     _bus
        jmp     _fuji_success


; void fuji_set_host_prefix(uint8_t hs, char *prefix);
;
_fuji_set_host_prefix:
        axinto  tmp7                    ; string memory location to read into
        setax   #t_fuji_set_host_prefix
        jmp     hp_common


.rodata
t_fuji_get_host_prefix:
        .byte $e0, $40, 0, 1, $ff, 0

t_fuji_set_host_prefix:
        .byte $e1, $80, 0, 1, $ff, 0
