        .export     _spn_find_printer

        .import     spn_printer
        .import     _spn_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _spn_find_printer
        ; look for spn_printer in devices
        setax   #spn_printer
        jmp     _spn_find_device
.endproc
