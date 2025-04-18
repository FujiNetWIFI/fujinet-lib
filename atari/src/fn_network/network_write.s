        .export     _network_write

        .import     _bus
        .import     _bus_status
        .import     _fn_device_error
        .import     _network_unit
        .import     _copy_network_cmd_data
        .import     popax

        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_write(const char *devicespec, const uint8_t *buf, uint16_t len)
_network_write:
        axinto  tmp7                    ; len

        ldy     #$00
        sty     _fn_device_error

        setax   #t_network_write

        jsr     _copy_network_cmd_data

        popax   IO_DCB::dbuflo          ; buf

        jsr     popax                   ; devicespec
        jsr     _network_unit           ; would be nice to skip this within the library if it's already done

        sta     IO_DCB::dunit
        sta     tmp6                    ; save unit
        setax   tmp7                    ; length into A/X, store it into IO_DCB
        sta     IO_DCB::dbytlo
        stx     IO_DCB::dbythi
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2

        jsr     _bus

        lda     tmp6                    ; restore the unit
        jmp     _bus_status

t_network_write:
        .byte 'W', $80, $ff, $ff, $ff, $ff
