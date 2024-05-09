        .export         _fuji_get_wifi_status

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_get_wifi_status(uint8_t *status)
;
; Return values are:
;  1: No SSID available
;  3: Connection Successful
;  4: Connect Failed
;  5: Connection lost
.proc _fuji_get_wifi_status
        axinto  tmp7
        setax   #t_fuji_get_wifi_status
        jsr     _copy_fuji_cmd_data
        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_get_wifi_status:
        .byte $fa, $40, $01, $00, $00, $00