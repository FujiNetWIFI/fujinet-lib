        .export     _sp_find_modem

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_modem
        ; look for sp_modem in devices
        setax   #sp_modem
        jmp     _sp_find_device
.endproc

.data
sp_modem:       .byte "MODEM", 0
