        .export     _sp_count
        .export     _sp_cmdlist
        .export     _sp_dest
        .export     _sp_dispatch_fn
        .export     _sp_error
        .export     _sp_is_init
        .export     _sp_payload

.data
; has the init routine been run?
_sp_is_init:        .byte 0


.bss

_sp_dest:           .res 1
_sp_error:          .res 1
_sp_count:          .res 2
_sp_dispatch_fn:    .res 2
_sp_cmdlist:        .res 10

; can this be reduced in size? not sure much more than 512 is used in library
_sp_payload:        .res 1024
