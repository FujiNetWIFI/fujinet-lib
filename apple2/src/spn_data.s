        .export     _spn_payload
        .export     _spn_count
        .export     _spn_cmdlist
        .export     _spn_dest
        .export     _spn_dispatch
        .export     _spn_error

        .export     spn_fn_d0
        .export     spn_printer
        .export     spn_network
        .export     spn_clock
        .export     spn_modem
        .export     spn_cpm

.bss
_spn_dest:      .res 1
_spn_error:     .res 1
_spn_count:     .res 2
_spn_dispatch:  .res 2
_spn_cmdlist:   .res 10
_spn_payload:   .res 1024

.data
spn_fn_d0:      .byte "FUJINET_DISK_0", 0
spn_printer:    .byte "PRINTER", 0
spn_network:    .byte "NETWORK", 0
spn_clock:      .byte "FN_CLOCK", 0
spn_modem:      .byte "MODEM", 0
spn_cpm:        .byte "CPM", 0