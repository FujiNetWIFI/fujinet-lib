        .export     _sp_find_clock

        .import     sp_clock
        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_clock
        ; look for sp_clock in devices
        setax   #sp_clock
        jmp     _sp_find_device
.endproc
