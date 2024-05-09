        .export     _network_close

        .import     _bus
        .import     _bus_status
        .import     _fn_device_error
        .import     _network_unit
        .import     _copy_network_cmd_data
        .import     popax

        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_close(char* devicespec)
.proc _network_close
        ldy     #$00
        sty     _fn_device_error

        ; get the network unit for this device, A/X already set correctly
        jsr     _network_unit
        sta     tmp8                    ; save the UNIT

        setax   #t_network_close
        jsr     _copy_network_cmd_data

        ; add our specifics, and call SIO
        mva     tmp8, IO_DCB::dunit
        ; mva     #$00, IO_DCB::dbuflo
        ; sta     IO_DCB::dbufhi
        jsr     _bus

        lda     tmp8
        jmp     _bus_status
.endproc

.rodata
t_network_close:
        .byte 'C', 0, 0, 0, 0, 0
