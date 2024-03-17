        .export     _sp_find_modem
        .export     _sp_modem_id

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_modem
        ; look for sp_modem in devices
        setax   #sp_modem
        bpl     no_error                ; could be 0 for not found, but not an error

        tax                             ; keep the error
        lda     #$00
        sta     _sp_modem_id            ; make sure id is cleared
        txa                             ; restore error
        rts                             ; we have to return -ve, as the +ve indicates the device id

no_error:
        tay
        ldx     #$00
        sta     _sp_modem_id
        tya                             ; ensure Z is set correctly
        rts
.endproc

.data
; the global MODEM unit id
_sp_modem_id:    .byte $00

sp_modem:       .byte "MODEM", 0
