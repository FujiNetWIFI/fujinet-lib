        .export     _main

        .import     _setup_sp
        .import     _spn_init

        .include    "macros.inc"

.proc _main
        jsr     _setup_sp
        jmp     _spn_init
.endproc
