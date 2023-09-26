        .export         _main
        .export         t_fn

; tests a function with signature:
;    [void|byte|word] function()
;    byte function()
;    word function()
.proc _main
        ; the return value just comes to caller in A, A/X if required. Nothing to do here to make it visible
        jmp     @run

@run:   jmp     (t_fn)

.endproc

.bss
t_fn:   .res 2
