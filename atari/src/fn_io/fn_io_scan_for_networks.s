        .export         _fn_io_scan_for_networks

        .import         fn_io_copy_cmd_data, _fn_io_do_bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fn_data.inc"

; int fn_io_scan_for_networks()
;
; returns count of networks scanned
; Uses tmp1-4 as the 4 byte buffer for scan data
.proc _fn_io_scan_for_networks
        setax   #t_io_scan_for_networks
        jsr     fn_io_copy_cmd_data
        mwa     #tmp9, IO_DCB::dbuflo
        jsr     _fn_io_do_bus

        lda     tmp9
        ldx     #$00
        rts
.endproc

.rodata
t_io_scan_for_networks:
        .byte $fd, $40, $04, $00, $00, $00
