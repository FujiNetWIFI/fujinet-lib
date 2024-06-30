        .export     _sp_dispatch
        .export     _sp_dispatch_address

        .import     _sp_count
        .import     _sp_cmdlist
        .import     _sp_error

; KEEP THIS FILE AS ASM AS IT DOES TRICKS WITH DATA AND INDIRECT CALLS TO DISPATCH FUNCTION

; int8_t sp_dispatch(uint8_t cmd)
;
; returns any error code from the smart port _sp_dispatch function
_sp_dispatch:
        sta     dispatch_data
        lda     #<_sp_cmdlist
        sta     dispatch_data+1
        lda     #>_sp_cmdlist
        sta     dispatch_data+2
        
        ; the SP dispatch alters the return address by 3 bytes to skip the data below.
        ; it returs with any error codes.
        .byte   $20             ; JSR - making this a byte so we can get exact location of address being called
_sp_dispatch_address:
        ; overwritten in sp_init to correct address
        .word   $0000

dispatch_data:
        .byte   $00             ; command
        .byte   $00             ; cmdlist low
        .byte   $00             ; cmdlist high

        ; A is error code. X,Y are the count of bytes
        sta     _sp_error
        stx     _sp_count
        sty     _sp_count+1

        ; convert to a return value
        ldx     #$00
        lda     _sp_error
        rts

