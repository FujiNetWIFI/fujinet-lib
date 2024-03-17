        .export         _fuji_set_ssid
        .import          copy_fuji_cmd_data, _bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-fuji.inc"
        .include        "device.inc"

; void  fuji_set_ssid(NetConfig *fuji_net_config)
; sends the ssid to bus.
.proc _fuji_set_ssid
        axinto  tmp7

        setax   #t_io_set_ssid
        jsr     copy_fuji_cmd_data

        ; copy mem location to DCB, and call bus
        mwa     tmp7, IO_DCB::dbuflo
        jmp     _bus
.endproc

.rodata
.define NCsz .sizeof(NetConfig)

t_io_set_ssid:
        .byte $fb, $80, <NCsz, >NCsz, $01, $00
