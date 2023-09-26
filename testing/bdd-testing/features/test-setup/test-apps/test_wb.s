        .export         _main
        .export         t_w1, t_b2, t_fn

        .import         pushax

        .include        "macros.inc"

; tests a function with signature:
;    [void|byte|word] function(word w1, byte b1)
.proc _main
        pushax  t_w1
        lda     t_b2

        jmp     @run

@run:   jmp     (t_fn)

.endproc

.bss
t_w1:   .res 2
t_b2:   .res 1

t_fn:   .res 2
