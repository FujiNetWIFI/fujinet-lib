        .export     _sp_payload
        .export     _sp_count
        .export     _sp_cmdlist
        .export     _sp_dest
        .export     _sp_dispatch
        .export     _sp_error

        .export     spn_fn_d0
        .export     spn_printer
        .export     spn_network
        .export     spn_clock
        .export     spn_modem
        .export     spn_cpm

.bss
_sp_dest:      .res 1
_sp_error:     .res 1
_sp_count:     .res 2
_sp_dispatch:  .res 2
_sp_cmdlist:   .res 10
_sp_payload:   .res 1024

.data
spn_fn_d0:      .byte "FUJINET_DISK_0", 0
spn_printer:    .byte "PRINTER", 0
spn_network:    .byte "NETWORK", 0
spn_clock:      .byte "FN_CLOCK", 0
spn_modem:      .byte "MODEM", 0
spn_cpm:        .byte "CPM", 0