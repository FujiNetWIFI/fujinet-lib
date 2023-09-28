        .export     _sp_find_modem

        .import     spn_modem
        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_modem
        ; look for spn_modem in devices
        setax   #spn_modem
        jmp     _sp_find_device
.endproc
