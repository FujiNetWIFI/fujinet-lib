        .export         _fuji_scan_for_networks

        .import         _bus
        .import         _fuji_success
        .import         copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_scan_for_networks(uint8_t *count)
;
; returns count of networks scanned
.proc _fuji_scan_for_networks
        axinto  tmp7
        setax   #t_fuji_scan_for_networks
        jsr     copy_fuji_cmd_data
        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success

.endproc

.rodata
t_fuji_scan_for_networks:
        .byte $fd, $40, $04, $00, $00, $00
