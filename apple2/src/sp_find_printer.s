        .export     _sp_find_printer

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_printer
        ; look for sp_printer in devices
        setax   #sp_printer
        jmp     _sp_find_device
.endproc

.data
sp_printer:     .byte "PRINTER", 0
