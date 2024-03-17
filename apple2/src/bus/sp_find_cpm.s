        .export     _sp_find_cpm
        .export     _sp_cpm_id

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_cpm
        ; look for sp_cpm in devices
        setax   #sp_cpm
        bpl     no_error                ; could be 0 for not found, but not an error

        tax                             ; keep the error
        lda     #$00
        sta     _sp_cpm_id              ; make sure id is cleared
        txa                             ; restore error
        rts                             ; we have to return -ve, as the +ve indicates the device id

no_error:
        tay
        ldx     #$00
        sta     _sp_cpm_id
        tya                             ; ensure Z is set correctly
        rts
.endproc

.data
; the global CPM unit id
_sp_cpm_id:    .byte $00

sp_cpm:         .byte "CPM", 0
