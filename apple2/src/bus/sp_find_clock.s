        .export     _sp_find_clock
        .export     _sp_clock_id

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_clock
        ; look for sp_clock in devices
        setax   #sp_clock
        bpl     no_error                ; could be 0 for not found, but not an error

        tax                             ; keep the error
        lda     #$00
        sta     _sp_clock_id            ; make sure id is cleared
        txa                             ; restore error
        rts                             ; we have to return -ve, as the +ve indicates the device id

no_error:
        tay
        ldx     #$00
        sta     _sp_clock_id
        tya                             ; ensure Z is set correctly
        rts
.endproc

.data
; the global CLOCK unit id
_sp_clock_id:    .byte $00

sp_clock:       .byte "FN_CLOCK", 0
