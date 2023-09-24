        .export     _network_open

        .import     _bus
        .import     _io_status
        .import     _network_unit
        .import     copy_cmd_data
        .import     popax

        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_open(char* devicespec, uint8_t trans)
.proc _network_open
        sta     tmp8                    ; trans
        popax   tmp6                    ; device_spec into A/X, and tmp6/7

        ; get the network unit for this device, A/X already set correctly
        jsr     _network_unit
        sta     tmp5                    ; save the UNIT

        setax   #t_network_open
        jsr     copy_cmd_data

        ; add our specifics, and call SIO
        mva     tmp5, IO_DCB::dunit
        mwa     tmp6, IO_DCB::dbuflo
        mva     tmp8, IO_DCB::daux2
        jsr     _bus

        jmp     _io_status
.endproc

.rodata
t_network_open:
        .byte 'O', $80, 0, 1, $0c, $ff
