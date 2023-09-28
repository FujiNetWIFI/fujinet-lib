        .export     _spn_find_cpm

        .import     spn_cpm
        .import     _spn_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _spn_find_cpm
        ; look for spn_cpm in devices
        setax   #spn_cpm
        jmp     _spn_find_device
.endproc
