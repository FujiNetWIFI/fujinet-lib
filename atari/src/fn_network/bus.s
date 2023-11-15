        .export     _bus

        .include    "atari.inc"

; void bus()
;
; For atari, calls SIOV

.proc _bus
        jmp     SIOV
.endproc
