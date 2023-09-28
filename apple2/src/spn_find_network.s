        .export     _spn_find_network

        .import     spn_network
        .import     _spn_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _spn_find_network
        ; look for spn_network in devices
        setax   #spn_network
        jmp     _spn_find_device
.endproc
