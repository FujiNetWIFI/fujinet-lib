        .export     network_rw

        .import     _bus
        .import     _io_status
        .import     _network_unit
        .import     copy_cmd_data
        .import     popax

        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_rw(char *devicespec, char *buf, void *dcb_table)
;
; This is a common routine used by both read and write calls.
.proc network_rw
        jsr     copy_cmd_data

        jsr     popax                   ; buf
        sta     IO_DCB::dbuflo
        stx     IO_DCB::dbufhi

        jsr     popax                   ; devicespec
        jsr     _network_unit           ; would be nice to skip this within the library if it's already done

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
.endproc