        .export     _fn_io_success
        .import     return0, return1

        .include    "device.inc"

; bool fn_io_success()
;
; returns 1 for success, 0 for error
.proc _fn_io_success
        lda     IO_DCB::dstats
        and     #$80
        beq     ok
        jmp     return0
ok:
        jmp     return1
.endproc