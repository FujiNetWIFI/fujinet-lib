        .export     _fn_device_error
        .export     _fn_device_error_ext1
        .export     _fn_device_error_ext2

.bss

; device specific error value
_fn_device_error:       .res 1

; allow for 2 additional codes to be saved
_fn_device_error_ext1:  .res 1
_fn_device_error_ext2:  .res 1

; memory for saving/restoring values
fn_save1:               .res 1
fn_save2:               .res 1
fn_save3:               .res 1
fn_save4:               .res 1
