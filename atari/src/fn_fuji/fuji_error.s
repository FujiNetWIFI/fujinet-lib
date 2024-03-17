        .export     _fuji_error
        .import     return0, return1

        .include    "device.inc"

; bool fuji_error()
;
; this should be called "is_error" as it returns true for errors, rather than true for success
; returns 0 for no error, 1 for any error
.proc _fuji_error
        lda     IO_DCB::dstats
        and     #$80
        bne     error
        jmp     return0
error:
        jmp     return1
.endproc