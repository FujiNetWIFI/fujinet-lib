        .export         _main
        .export         t_w1, t_b2, t_b3, t_fn

        .import         pusha
        .import         pushax

        .include        "macros.inc"

; tests a function with signature:
;    [void|byte|word] function(word w1, byte b2, byte b3)
.proc _main
        pushax  t_w1
        pusha   t_b2
        lda     t_b3

        jmp     @run

@run:   jmp     (t_fn)

.endproc

.bss
t_w1:   .res 2
t_b2:   .res 1
t_b3:   .res 1

t_fn:   .res 2
