        .export         _main
        .export         t_w1, t_w2, t_fn

        .import         pushax

        .include        "macros.inc"

; tests a function with signature:
;    [void|byte|word] function(word w1, word w2)
.proc _main
        pushax  t_w1
        setax   t_w2

        jmp     @run

@run:   jmp     (t_fn)

.endproc

.bss
t_w1:   .res 2
t_w2:   .res 2

t_fn:   .res 2
