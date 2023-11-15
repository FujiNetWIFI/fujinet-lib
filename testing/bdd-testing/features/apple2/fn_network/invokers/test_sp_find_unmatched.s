        .export     _main

        .import     _sp_find_device
        .import     _sp_init
        .import     spe_should_fail_device_lookup

        .include    "macros.inc"

.proc _main
        jsr     _sp_init
        setax   #unmatched_name
        jmp     _sp_find_device
.endproc

.data
unmatched_name:   .byte "X",0
