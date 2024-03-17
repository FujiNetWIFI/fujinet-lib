        .export     _sp_find_network
        .export     _sp_network

        .import     _fn_error
        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

; int8_t sp_find_network();
;
; returns the ID of the Network device, if available. 0 if not found, negative if there was an error.

.proc _sp_find_network
        ; look for sp_network in devices
        setax   #sp_network_s
        jsr     _sp_find_device
        bpl     no_error                ; could be 0 for not found, but not an error

        tax                             ; keep the error
        lda     #$00
        sta     _sp_network             ; make sure id is cleared
        txa                             ; restore error
        rts                             ; we have to return -ve, as the +ve indicates the device id

no_error:
        tay
        ldx     #$00
        sta     _sp_network
        tya                             ; ensure Z is set correctly
        rts

.endproc


.data
; the global NETWORK unit id
_sp_network:    .byte $00

sp_network_s:   .byte "NETWORK", 0
