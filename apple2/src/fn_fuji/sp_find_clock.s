        .export     _sp_find_clock

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_clock
        ; look for sp_clock in devices
        setax   #sp_clock
        jmp     _sp_find_device
.endproc

.data
sp_clock:       .byte "FN_CLOCK", 0
