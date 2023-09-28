        .export     _main

        .import     _setup_sp
        .import     _sp_find_fuji
        .import     _sp_init

        .include    "macros.inc"

.proc _main
        jsr     _setup_sp
        jsr     _sp_init
        jmp     _sp_find_fuji
.endproc
