        .export         _main
        .export         t_b1, t_b2, t_fn

        .import         pusha

        .include        "macros.inc"

; tests a function with signature:
;    [void|byte|word] function(byte b1, byte w2)
.proc _main
        pusha   t_b1
        lda     t_b2

        jmp     @run

@run:   jmp     (t_fn)

.endproc

.bss
t_b1:   .res 1
t_b2:   .res 1

t_fn:   .res 2
