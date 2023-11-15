        .export         _fn_io_set_ssid
        .import          fn_io_copy_cmd_data, _fn_io_do_bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-io.inc"
        .include        "fn_data.inc"

; void  fn_io_set_ssid(NetConfig *fn_io_net_config)
; sends the ssid to bus.
.proc _fn_io_set_ssid
        axinto  tmp7

        setax   #t_io_set_ssid
        jsr     fn_io_copy_cmd_data

        ; copy mem location to DCB, and call bus
        mwa     tmp7, IO_DCB::dbuflo
        jmp     _fn_io_do_bus
.endproc

.rodata
.define NCsz .sizeof(NetConfig)

t_io_set_ssid:
        .byte $fb, $80, <NCsz, >NCsz, $01, $00
