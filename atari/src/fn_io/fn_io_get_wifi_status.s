        .export         _fn_io_get_wifi_status
        .import         fn_io_copy_cmd_data, _fn_io_do_bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; int fn_io_get_wifi_status()
;
; Return values are:
;  1: No SSID available
;  3: Connection Successful
;  4: Connect Failed
;  5: Connection lost
.proc _fn_io_get_wifi_status
        setax   #t_io_get_wifi_status
        jsr     fn_io_copy_cmd_data
        mwa     #tmp9, IO_DCB::dbuflo
        jsr     _fn_io_do_bus

        ldx     #$00
        lda     tmp9
        rts
.endproc

.rodata
t_io_get_wifi_status:
        .byte $fa, $40, $01, $00, $00, $00