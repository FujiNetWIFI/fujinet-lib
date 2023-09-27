        .export     _main

        .import     _setup_sp
        .import     _sp_init

        .include    "macros.inc"

.proc _main
        jsr     _setup_sp
        jmp     _sp_init
.endproc
