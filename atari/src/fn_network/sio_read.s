        .export     _sio_read

        .import     _bus
        .import     _fn_bytes_read
        .import     _io_status
        .import     copy_cmd_data
        .import     popa
        .import     popax

        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t sio_read(uint8_t unit, void * buffer, uint16_t len)

_sio_read:
        axinto  ptr1                    ; length
        axinto  _fn_bytes_read

        setax   #t_network_read
        jsr     copy_cmd_data           ; setup DCB

        popax   IO_DCB::dbuflo          ; buffer arg
        popa    IO_DCB::dunit           ; unit arg
        setax   ptr1                    ; length
        sta     IO_DCB::dbytlo
        stx     IO_DCB::dbythi
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2
        jsr     _bus

        lda     IO_DCB::dunit           ; restore the unit
        jmp     _io_status

.rodata
t_network_read:
        .byte 'R', $40, $ff, $ff, $ff, $ff
