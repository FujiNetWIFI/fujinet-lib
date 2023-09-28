        .export     _spn_find_clock

        .import     spn_clock
        .import     _spn_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _spn_find_clock
        ; look for spn_clock in devices
        setax   #spn_clock
        jmp     _spn_find_device
.endproc
