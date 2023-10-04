        .export         _main
        .export         t_b1, t_w2, t_w3, t_w4, t_fn

        .import         pusha
        .import         pushax

        .include        "macros.inc"

; tests a function with signature:
;    [void|byte|word] function(byte w1, word w2, word w3, word w4)
.proc _main
        pusha   t_b1
        pushax  t_w2
        pushax  t_w3
        setax   t_w4

        jmp     @run

@run:   jmp     (t_fn)

.endproc

.bss
t_b1:   .res 1
t_w2:   .res 2
t_w3:   .res 2
t_w4:   .res 2

t_fn:   .res 2
