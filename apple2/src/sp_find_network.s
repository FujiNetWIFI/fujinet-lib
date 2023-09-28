        .export     _sp_find_network

        .import     spn_network
        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_network
        ; look for spn_network in devices
        setax   #spn_network
        jmp     _sp_find_device
.endproc
