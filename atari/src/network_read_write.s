        .export     _network_read
        .export     _network_write

        .import     _bus
        .import     _io_status
        .import     _network_unit
        .import     copy_cmd_data
        .import     popax

        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_read(char* devicespec, uint8_t *buf, uint16_t len)
_network_read:
        jsr     io_params
        setax   #t_network_read
        jmp     do_call

; uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len)
_network_write:
        jsr     io_params
        setax   #t_network_write
        ; fall through

do_call:
        jsr     copy_cmd_data

        ; add our specifics, and call SIO
        mva     tmp4, IO_DCB::dunit
        mwa     tmp5, IO_DCB::dbuflo
        setax   tmp7
        sta     IO_DCB::dbytlo
        stx     IO_DCB::dbythi
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2
        jsr     _bus

        jmp     _io_status
        ; implicit rts

io_params:
        axinto  tmp7                    ; len -> tmp7/8
        popax   tmp5                    ; buf -> tmp5/6
        jsr     popax                   ; get devicespec into A/X for call to network_unit

        ; get the network unit for this device, A/X already set correctly
        jsr     _network_unit
        sta     tmp4                    ; save the UNIT
        rts

.rodata
t_network_read:
        .byte 'R', $40, $ff, $ff, $ff, $ff

t_network_write:
        .byte 'W', $80, $ff, $ff, $ff, $ff
