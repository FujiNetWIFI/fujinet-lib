        .export     _network_read

        .import     _bus
        .import     _fn_bytes_read
        .import     _fn_device_error
        .import     _fn_network_bw
        .import     _fn_network_conn
        .import     _fn_network_error
        .import     _io_status
        .import     _network_status_unit
        .import     _network_unit
        .import     copy_cmd_data
        .import     popax
        .import     pusha
        .import     pushax

        .include    "fujinet-network.inc"
        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_read(char* devicespec, uint8_t *buf, uint16_t len)
_network_read:
        axinto  tmp2                    ; len, tmp2/3

        ldy     #$00
        sty     _fn_device_error

        popax   ptr4                    ; buf
        jsr     popax                   ; device spec, only need unit from it
        jsr     _network_unit           ; unit
        sta     tmp1

        ; is the length 0?
        lda     tmp2
        ora     tmp3
        bne     :+

        ; 0 bytes specified, return an error
        ldx     #$00
        lda     #FN_ERR_BAD_CMD
        rts

        ; do a status first to determine number of bytes available
:       jsr     pusha                   ; unit
        pushax  #_fn_network_bw         ; bytes waiting location
        pushax  #_fn_network_conn       ; connection status
        setax   #_fn_network_error      ; network error
        jsr     _network_status_unit
        ; TODO: errors?

        lda     tmp3                    ; hi byte of length asked for
        cmp     _fn_network_bw+1        ; compare with the hi byte of bytes available
        bne     :+                      ; we don't need to compare anything else if they are different
        lda     tmp2                    ; lo byte of length asked for
        cmp     _fn_network_bw          ; lo byte of bytes available
:       bcc     lower                   ; len < bytes waiting

        ; we know that bw <= len, so take bw count bytes instead
        mwa     _fn_network_bw, ptr3    ; only use BW count
        bcs     :+

lower:  mwa     tmp2, ptr3              ; use len asked for, as bw is larger, but we don't want all of it

:       mwa     ptr3, _fn_bytes_read    ; keep track of the count we are reading
        setax   #t_network_read
        jsr     copy_cmd_data

        setax   ptr4                    ; buf
        sta     IO_DCB::dbuflo
        stx     IO_DCB::dbufhi

        lda     tmp1
        sta     IO_DCB::dunit
        setax   ptr3                    ; length into A/X, store it into IO_DCB
        sta     IO_DCB::dbytlo
        stx     IO_DCB::dbythi
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2

        jsr     _bus

        lda     tmp1                    ; restore the unit
        jmp     _io_status

.rodata
t_network_read:
        .byte 'R', $40, $ff, $ff, $ff, $ff
