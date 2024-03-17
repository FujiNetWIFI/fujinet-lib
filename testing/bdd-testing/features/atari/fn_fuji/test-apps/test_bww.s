        .export         _main
        .export         t_b1, t_w2, t_w3, t_fn

        .import         pushax, pusha

        .include        "macros.inc"

; tests a function with signature:
;    [void|byte|word] function(byte b1, word w2, word w3)
.proc _main
        pusha   t_b1
        pushax  t_w2
        setax   t_w3

        jmp     @run

@run:   jmp     (t_fn)

.endproc

.bss
t_b1:   .res 1
t_w2:   .res 2
t_w3:   .res 2

t_fn:   .res 2
