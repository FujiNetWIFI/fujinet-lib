        .export         _fuji_get_ssid

        .import         _bus
        .import         _fuji_success
        .import         copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-fuji.inc"
        .include        "device.inc"

; bool _fuji_get_ssid(NetConfig *net_config)
;
; read ssid to fuji_net_config
.proc _fuji_get_ssid
        axinto  tmp7

        setax   #t_fuji_get_ssid
        jsr     copy_fuji_cmd_data

        ; copy mem location to DCB, and call bus
        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
.define NCsz .sizeof(NetConfig)

t_fuji_get_ssid:
        .byte $fe, $40, <NCsz, >NCsz, $00, $00
