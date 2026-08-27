        .export         _fuji_qrcode_encode
        .export         _fuji_qrcode_length

        .import         _copy_fuji_cmd_data
        .import         _bus
        .import         _fuji_success
        .import         popa

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; tmp5/tmp6 are used to hold arguments across _copy_fuji_cmd_data, which
; documents that it trashes tmp9/tmp10.

; bool fuji_qrcode_encode(uint8_t version, uint8_t ecc, bool shorten);
;
; cc65 passes the last argument in A, so `shorten` arrives there and the other
; two come off the stack. `shorten` is discarded: an SIO command frame carries
; only aux1 and aux2, so there is no third parameter byte to put it in, and the
; FujiNet reads it as zero on this bus.
;
_fuji_qrcode_encode:
        jsr     popa                    ; ecc
        sta     tmp5
        jsr     popa                    ; version
        sta     tmp6

        setax   #t_fuji_qrcode_encode
        jsr     _copy_fuji_cmd_data

        mva     tmp6, IO_DCB::daux1     ; version
        mva     tmp5, IO_DCB::daux2     ; ecc
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success

; bool fuji_qrcode_length(uint8_t output_mode, unsigned long *len);
;
_fuji_qrcode_length:
        axinto  tmp7                    ; pointer to len in tmp7/8
        jsr     popa                    ; output_mode
        sta     tmp5

        setax   #t_fuji_qrcode_length
        jsr     _copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        mva     tmp5, IO_DCB::daux1     ; output mode
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success


.rodata
; DCOMND, DSTATS, DBYTLO, DBYTHI, DAUX1, DAUX2
; aux bytes are filled in at runtime above.
t_fuji_qrcode_encode:
        .byte $bd, $00, 0, 0, 0, 0

t_fuji_qrcode_length:
        .byte $be, $40, 4, 0, 0, 0
