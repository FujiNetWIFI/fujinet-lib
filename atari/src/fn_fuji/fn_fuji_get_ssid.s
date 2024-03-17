        .export         _fn_fuji_get_ssid
        .import         copy_fuji_cmd_data, _bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-fuji.inc"
        .include        "device.inc"

; void _fn_fuji_get_ssid(NetConfig *net_config)
;
; read ssid to fn_fuji_net_config
.proc _fn_fuji_get_ssid
        axinto  tmp7

        setax   #t_io_get_ssid
        jsr     copy_fuji_cmd_data

        ; copy mem location to DCB, and call bus
        mwa     tmp7, IO_DCB::dbuflo
        jmp     _bus
.endproc

.rodata
.define NCsz .sizeof(NetConfig)

t_io_get_ssid:
        .byte $fe, $40, <NCsz, >NCsz, $00, $00
