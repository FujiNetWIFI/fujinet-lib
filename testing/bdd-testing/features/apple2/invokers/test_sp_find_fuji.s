        .export     _main

        .import     _setup_sp
        .import     _spn_find_fuji
        .import     _spn_init

        .include    "macros.inc"

.proc _main
        jsr     _setup_sp
        jsr     _spn_init
        jmp     _spn_find_fuji
.endproc
