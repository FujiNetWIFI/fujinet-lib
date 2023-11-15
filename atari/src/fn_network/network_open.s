        .export     _network_open

        .import     _bus
        .import     _fn_device_error
        .import     _io_status
        .import     _network_unit
        .import     copy_nw_cmd_data
        .import     fn_open_mode_table
        .import     fn_open_trans_table
        .import     popa
        .import     popax

        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans);
.proc _network_open
        sta     tmp8                    ; save trans

        ldy     #$00
        sty     _fn_device_error

        setax   #t_network_open
        jsr     copy_nw_cmd_data

        jsr     popa                    ; mode
        sta     IO_DCB::daux1

        jsr     popax                   ; devicespec
        sta     IO_DCB::dbuflo
        stx     IO_DCB::dbufhi

        ; get the network unit for this device
        jsr     _network_unit
        sta     IO_DCB::dunit
        pha

        ; save mode in modes table with offset of unit
        ; this allows other users of this unit to see the mode set
        tax
        mva     IO_DCB::daux1, {fn_open_mode_table-1, x}

        lda     tmp8                    ; trans
        sta     IO_DCB::daux2

        ; store the translation mode for this device
        sta     fn_open_trans_table-1, x

        jsr     _bus
        pla
        jmp     _io_status
.endproc

.rodata
t_network_open:
        .byte 'O', $80, 0, 1, $ff, $ff
