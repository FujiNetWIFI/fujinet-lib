        .export     _main

        .import     _sp_init

        .include    "macros.inc"

.proc _main
        jmp     _sp_init
.endproc
