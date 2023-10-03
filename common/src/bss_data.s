        .export     _fn_device_error
        .export     _fn_device_error_ext1
        .export     _fn_device_error_ext2
        .export     fn_open_mode_table

.bss

; device specific error value
_fn_device_error:       .res 1

; allow for 2 additional codes to be saved
_fn_device_error_ext1:  .res 1
_fn_device_error_ext2:  .res 1

; index into this to get the currently open channel's mode, set when open is called
fn_open_mode_table:     .res 8
