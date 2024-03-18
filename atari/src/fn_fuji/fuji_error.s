        .export     _fuji_error
        .export     fuji_error_cmp
        .import     return0, return1

        .include    "device.inc"

; bool fuji_error()
;
; this should be called "is_error" as it returns true for errors, rather than true for success
;
; There are 2 thoughts on this, in original CONFIG code, it always checks if dstats > 128
; but other sources indicate anything other than 1 is an error, and is the error code.
; 
; returns 0 for no error (false), 1 for any error (true)
_fuji_error:
        lda     IO_DCB::dstats

fuji_error_cmp:
        cmp     #$01
        bne     error
        jmp     return0
error:
        jmp     return1
