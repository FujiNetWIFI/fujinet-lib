        .export     _fn_error

        .import     _fn_device_error
        .import     fuji_error_cmp

        .include    "device.inc"

; uint8_t fn_error(uint8_t code)
;
; Converts DCB status code into fujinet-network error.
; Keeps original status in _fn_device_error.

; TODO: Needs work to translate between DCB codes to FN specific versions.
; The apple2 version looks at lots of codes and sets appropriate values, but here we only use OK/IO_ERROR.
;
; At the moment we simply pass to fuji_error which checks if the value given (probably always dstats) is 1 for success, other for error
; and returns 0/false (by design FN_ERR_OK), or 1/true (by design FN_ERR_IO_ERROR)
; which are inverted but correct. Most functions return the success status (0 = false = error), but we flip that into FN_ERR_OK/FN_ERR_IO_ERROR

.proc _fn_error
        sta     _fn_device_error
        jmp     fuji_error_cmp
.endproc
