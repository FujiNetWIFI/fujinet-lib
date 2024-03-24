        .export     _fn_bytes_read
        .export     _fn_device_error
        .export     _fn_network_error
        .export     _fn_network_bw
        .export     _fn_network_conn

; -------------------------------------------------
; BSS
; -------------------------------------------------
.bss

; device specific error value, e.g. SmartPort specific errors
_fn_device_error:       .res 1

;; network status values, these come from FujiNet itself, not the host machine.

; general network error
_fn_network_error:      .res 1
; bytes waiting
_fn_network_bw:         .res 2
; connection status
_fn_network_conn:       .res 1

; count of bytes read in last network_read call, so applications can nul terminate as needed.
_fn_bytes_read:         .res 2
