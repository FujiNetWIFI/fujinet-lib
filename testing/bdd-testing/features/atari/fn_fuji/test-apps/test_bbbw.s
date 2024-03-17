        .export         _main
        .export         t_b1, t_b2, t_b3, t_w4, t_fn

        .import         pushax, pusha

        .include        "macros.inc"

; tests a function with signature:
;    [void|byte|word] function(byte b1, byte b2, byte b3, word w4)
.proc _main
        pusha   t_b1
        pusha   t_b2
        pusha   t_b3
        setax   t_w4

        jmp     @run

@run:   jmp     (t_fn)

.endproc

.bss
t_b1:   .res 1
t_b2:   .res 1
t_b3:   .res 1
t_w4:   .res 2

t_fn:   .res 2
