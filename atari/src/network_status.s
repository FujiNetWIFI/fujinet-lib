        .export     _network_status
        .export     _network_status_unit

        .import     _bus
        .import     _network_unit
        .import     copy_cmd_data

        .include    "atari.inc"
        .include    "device.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_status(char *devicespec)
;
_network_status:
        ; get the network unit for this device, A/X already set correctly
        jsr     _network_unit
        ; fall through to known unit case

; uint8_t network_status_unit(uint8_t unit)
;
_network_status_unit:
        sta     tmp5                    ; save the UNIT

        setax   #t_network_status
        jsr     copy_cmd_data

        mva     tmp5, IO_DCB::dunit
        mwa     #DVSTAT, IO_DCB::dbuflo
        jsr     _bus
        ldx     #$00
        lda     EXTENDED_ERROR
        rts


.rodata
t_network_status:
        .byte 'S', $40, 4, 0, 0, 0
