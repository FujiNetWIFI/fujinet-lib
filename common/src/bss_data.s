        .export     _fn_device_error
        .export     _fn_network_error
        .export     _fn_network_bw
        .export     _fn_network_conn

.bss

; device specific error value, e.g. SmartPort specific errors
_fn_device_error:       .res 1


;; network status values, these come from FujiNet itself, not the host machine.

; general network error
_fn_network_error:      .res 1
; bytes waiting
_fn_network_bw:         .res 1
; connection status
_fn_network_conn:       .res 1
