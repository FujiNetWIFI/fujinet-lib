        .export     _sp_payload
        .export     _sp_count
        .export     _sp_cmdlist
        .export     _sp_dest
        .export     _sp_dispatch
        .export     _sp_error

        .export     sp_fn_d0
        .export     sp_printer
        .export     sp_network
        .export     sp_clock
        .export     sp_modem
        .export     sp_cpm

.bss
_sp_dest:      .res 1
_sp_error:     .res 1
_sp_count:     .res 2
_sp_dispatch:  .res 2
_sp_cmdlist:   .res 10
_sp_payload:   .res 1024

.data
sp_fn_d0:      .byte "FUJINET_DISK_0", 0
sp_printer:    .byte "PRINTER", 0
sp_network:    .byte "NETWORK", 0
sp_clock:      .byte "FN_CLOCK", 0
sp_modem:      .byte "MODEM", 0
sp_cpm:        .byte "CPM", 0