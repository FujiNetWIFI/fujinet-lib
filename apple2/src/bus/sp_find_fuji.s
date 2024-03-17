        .export     _sp_find_fuji
        .export     _sp_fuji_id

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

; int8_t sp_find_fuji();
;
; returns the ID of the FUJI device, if available. 0 if not found, negative if there was an error.

.proc _sp_find_fuji
        ; look for sp_fn_d0 in devices - this is a hack in firmware.
        setax   #sp_fn_d0
        jsr     _sp_find_device
        tay                             ; keep the result
        bpl     no_error                ; could be 0 for not found, but not an error

        lda     #$00
        sta     _sp_fuji_id             ; make sure id is cleared
        tax
        tya                             ; restore error
        rts                             ; we have to return -ve, as the +ve indicates the device id

no_error:
        sta     _sp_fuji_id
        ldx     #$00
        tya                             ; ensure Z is set correctly
        rts

.endproc

.data
; the global FUJI unit id
_sp_fuji_id:    .byte $00

; the disk 0 is where to send fuji commands... for now!
sp_fn_d0:       .byte "FUJINET_DISK_0", 0
