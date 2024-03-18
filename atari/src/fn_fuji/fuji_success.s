        .export     _fuji_success
        .import     return0, return1

        .include    "device.inc"

; bool fuji_success()
;
; returns 1 for success, 0 for error
.proc _fuji_success
        lda     IO_DCB::dstats
        cmp     #$01
        beq     ok
        jmp     return0
ok:
        jmp     return1
.endproc