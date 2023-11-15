        .export     _sp_find_network
        .export     _sp_network

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_network
        ; look for sp_network in devices
        setax   #sp_network_s
        jmp     _sp_find_device
.endproc


.data
; the global NETWORK unit id
_sp_network:    .byte $00

sp_network_s:   .byte "NETWORK", 0
