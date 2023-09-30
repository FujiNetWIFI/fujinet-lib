        .export     _network_status
        .export     _network_status_unit

        .import     _bus
        .import     _network_unit
        .import     copy_cmd_data
        .import     incsp2
        .import     popax

        .include    "atari.inc"
        .include    "device.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c)
;
_network_status:
        ; initially ignoring the c/bw params
        jsr     incsp2          ; drop bw

        ; get the network unit for this device
        jsr     popax
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
