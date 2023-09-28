        .export     _sp_find_cpm

        .import     spn_cpm
        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_cpm
        ; look for spn_cpm in devices
        setax   #spn_cpm
        jmp     _sp_find_device
.endproc
