        .export     _network_status
        .export     _network_status_unit

        .import     _bus
        .import     _fn_error
        .import     _network_unit
        .import     copy_cmd_data
        .import     incsp4
        .import     popa
        .import     popax
        .import     return0
        .import     return1

        .include    "atari.inc"
        .include    "device.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
;
; Return will be either FN_ERR_OK, or FN_ERR_IO_ERROR.
; Actual device error is stored in err
_network_status:
        axinto  ptr1            ; err location
        popax   ptr2            ; connected status location
        popax   ptr3            ; bytes waiting location

        jsr     popax           ; devicespec
        jsr     _network_unit
        jmp     common

; uint8_t network_status_unit(uint8_t unit, uint16_t *bw, uint8_t *c, uint8_t *err)
;
_network_status_unit:
        axinto  ptr1            ; err location
        popax   ptr2            ; connected status location
        popax   ptr3            ; bytes waiting location
        jsr     popa            ; unit

common:
        sta     tmp8                    ; save the UNIT

        setax   #t_network_status
        jsr     copy_cmd_data

        mva     tmp8, IO_DCB::dunit
        mwa     #DVSTAT, IO_DCB::dbuflo
        jsr     _bus

        ldy     #$00
        ; copy DVSTAT values to pointers passed to us
        lda     DVSTAT+3                ; this is extended error location, but only because it's part of DVSTAT return from call!
        sta     (ptr1), y               ; device error
        lda     DVSTAT+2
        sta     (ptr2), y               ; connected status
        mway    DVSTAT, {(ptr3), y}     ; waiting bytes count

        ; convert dstats to a library error
        lda     IO_DCB::dstats
        jmp     _fn_error

.rodata
t_network_status:
        .byte 'S', $40, 4, 0, 0, 0
