        .export     _sp_find_clock

        .import     spn_clock
        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_clock
        ; look for spn_clock in devices
        setax   #spn_clock
        jmp     _sp_find_device
.endproc
