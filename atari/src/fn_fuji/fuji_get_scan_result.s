        .export         _fuji_get_scan_result

        .import         _bus
        .import         _fuji_success
        .import         copy_fuji_cmd_data
        .import         popa

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-fuji.inc"
        .include        "device.inc"

; bool fuji_get_scan_result(uint8_t network_index, SSIDInfo *ssid_info)
;
; caller must supply memory location for ssidinfo to go
.proc _fuji_get_scan_result
        axinto  tmp7            ; location to put ssidinfo into

        setax   #t_fuji_get_scan_result
        jsr     copy_fuji_cmd_data

        jsr     popa            ; network index
        sta     IO_DCB::daux1
        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
.define SIsz .sizeof(SSIDInfo)

t_fuji_get_scan_result:
        .byte $fc, $40, <SIsz, >SIsz, $ff, $00
