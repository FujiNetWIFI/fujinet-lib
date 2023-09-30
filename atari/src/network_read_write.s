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
        axinto  tmp7                    ; len
        setax   #t_network_read
        jmp     do_call

; uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len)
_network_write:
        axinto  tmp7                    ; len
        setax   #t_network_write
        ; fall through

do_call:
        jsr     copy_cmd_data

        jsr     popax                   ; buf
        sta     IO_DCB::dbuflo
        stx     IO_DCB::dbufhi
        jsr     popax                   ; devicespec
        jsr     _network_unit
        sta     IO_DCB::dunit
        sta     tmp6                    ; save unit
        lda     tmp7
        ldx     tmp8
        sta     IO_DCB::dbytlo
        stx     IO_DCB::dbythi
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2

        jsr     _bus
        lda     tmp6                    ; restore the unit
        jmp     _io_status

.rodata
t_network_read:
        .byte 'R', $40, $ff, $ff, $ff, $ff

t_network_write:
        .byte 'W', $80, $ff, $ff, $ff, $ff
