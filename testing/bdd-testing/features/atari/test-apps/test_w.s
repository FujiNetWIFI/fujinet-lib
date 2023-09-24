        .export         _main
        .export         t_w1, t_fn

        .include        "macros.inc"

; tests a function with signature:
;    [void|byte|word] function(word b1)
.proc _main
        setax   t_w1
        jmp     @run

@run:   jmp     (t_fn)

.endproc

.bss
t_w1:   .res 2

t_fn:   .res 2
