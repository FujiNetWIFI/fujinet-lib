        .export     _sp_find_network

        .import     sp_network
        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_network
        ; look for sp_network in devices
        setax   #sp_network
        jmp     _sp_find_device
.endproc
