        .export     _network_status
        .export     _network_status_unit

        .import     _bus
        .import     _fn_device_error
        .import     _fn_device_error_ext1
        .import     _fn_error
        .import     _network_unit
        .import     copy_cmd_data
        .import     incsp4
        .import     popax
        .import     return0
        .import     return1

        .include    "atari.inc"
        .include    "device.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
;
; TODO: Support bw/c/err parameters
; Return will be either FN_ERR_OK, or FN_ERR_IO_ERROR.
; Actual device error is stored in _fn_device_error_ext1, and _fn_device_error
_network_status:
        ; initially ignoring the c/bw/err params
        jsr     incsp4          ; drop 'bw', and 'c' parameters (2 bytes each). err is just in a/x, so ignored anyway

        jsr     popax           ; devicespec
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

        ; handle return - set device specific errors for caller to access if needed, then return generic error
        lda     EXTENDED_ERROR
        sta     _fn_device_error_ext1
        lda     IO_DCB::dstats
        jmp     _fn_error

.rodata
t_network_status:
        .byte 'S', $40, 4, 0, 0, 0
