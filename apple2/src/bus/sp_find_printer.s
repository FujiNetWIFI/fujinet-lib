        .export     _sp_find_printer
        .export     _sp_printer_id

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_printer
        ; look for sp_printer in devices
        setax   #sp_printer
        bpl     no_error                ; could be 0 for not found, but not an error

        tax                             ; keep the error
        lda     #$00
        sta     _sp_printer_id          ; make sure id is cleared
        txa                             ; restore error
        rts                             ; we have to return -ve, as the +ve indicates the device id

no_error:
        tay
        ldx     #$00
        sta     _sp_printer_id
        tya                             ; ensure Z is set correctly
        rts
.endproc

.data
; the global PRINTER unit id
_sp_printer_id:    .byte $00

sp_printer:     .byte "PRINTER", 0
