        .export     _network_close

        .import     _bus
        .import     _io_status
        .import     _network_unit
        .import     copy_cmd_data
        .import     popax

        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_close(char* devicespec)
.proc _network_close
        ; get the network unit for this device, A/X already set correctly
        jsr     _network_unit
        sta     tmp6                    ; save the UNIT

        setax   #t_network_close
        jsr     copy_cmd_data

        ; add our specifics, and call SIO
        mva     tmp6, IO_DCB::dunit
        mva     #$00, IO_DCB::dbuflo
        sta     IO_DCB::dbufhi
        jsr     _bus

        jmp     _io_status
.endproc

.rodata
t_network_close:
        .byte 'C', 0, 0, 0, 0, 0
