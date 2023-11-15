        .export     _network_read

        .import     _fn_device_error
        .import     _fn_network_bw
        .import     _fn_network_conn
        .import     _fn_network_error
        .import     _network_status_unit
        .import     _network_unit
        .import     _sio_read
        .import     popax
        .import     pusha
        .import     pushax

        .include    "fujinet-network.inc"
        .include    "device.inc"
        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_read(char* devicespec, uint8_t *buf, uint16_t len)

; TODO: Work out LARGE transfers, like apple2 does in 512 byte chunks. Or does SIO allow single large chunks?

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

        ; -------------------------------------------------------------------------------------------------------------
        ; DECIDE ACTUAL COUNT, GET BYTES AVAILABLE
        ; -------------------------------------------------------------------------------------------------------------

:       pusha   tmp1                    ; unit
        pushax  #_fn_network_bw         ; bytes waiting location
        pushax  #_fn_network_conn       ; connection status
        setax   #_fn_network_error      ; network error
        jsr     _network_status_unit

        lda     tmp3                    ; hi byte of length asked for
        cmp     _fn_network_bw+1        ; compare with the hi byte of bytes available
        bne     :+                      ; we don't need to compare anything else if they are different
        lda     tmp2                    ; lo byte of length asked for
        cmp     _fn_network_bw          ; lo byte of bytes available
:       bcc     lower                   ; len < bytes waiting, so can use it for read count

        mwa     _fn_network_bw, tmp2    ; only use BW count, as it's lower than len
        bcs     :+

lower:
        ; -------------------------------------------------------------------------------------------------------------
        ; PERFORM READ
        ; -------------------------------------------------------------------------------------------------------------
:       pusha   tmp1                    ; unit
        pushax  ptr4                    ; buffer
        setax   tmp2
        jmp     _sio_read
