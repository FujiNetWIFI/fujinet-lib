        .export     _sp_count
        .export     _sp_cmdlist
        .export     _sp_dest
        .export     _sp_error
        .export     _sp_is_init
        .export     _sp_nw_unit
        .export     _sp_payload

        .include    "sp.inc"

.data
; has the init routine been run?
_sp_is_init:        .byte 0

; the network sub-device id matching Nx: ranging from 1 to 8. 0 means it hasn't been specified in an open.
_sp_nw_unit:        .byte 0

.bss

_sp_dest:           .res 1
_sp_error:          .res 1
_sp_count:          .res 2
_sp_cmdlist:        .res 10

_sp_payload:        .res SP_PAYLOAD_SIZE
