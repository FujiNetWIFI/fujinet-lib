        .export     _fn_io_error
        .import     return0, return1

        .include    "fn_data.inc"

; bool fn_io_error()
;
; returns 0 for no error, 1 for any error
.proc _fn_io_error
        lda     IO_DCB::dstats
        and     #$80
        bne     error
        jmp     return0
        ; implicit rts
error:
        jmp     return1
        ; implicit rts
.endproc