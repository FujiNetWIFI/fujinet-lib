        .export         _fn_io_get_scan_result
        .import         fn_io_copy_cmd_data, _fn_io_do_bus
        .import         popa

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fujinet-io.inc"
        .include        "fn_data.inc"

; void fn_io_get_scan_result(uint8_t network_index, SSIDInfo *ssid_info)
;
; caller must supply memory location for ssidinfo to go
.proc _fn_io_get_scan_result
        axinto  tmp7            ; location to put ssidinfo into

        setax   #t_io_get_scan_result
        jsr     fn_io_copy_cmd_data

        jsr     popa            ; network index
        sta     IO_DCB::daux1
        mwa     tmp7, IO_DCB::dbuflo
        jmp     _fn_io_do_bus
.endproc

.rodata
.define SIsz .sizeof(SSIDInfo)

t_io_get_scan_result:
        .byte $fc, $40, <SIsz, >SIsz, $ff, $00
