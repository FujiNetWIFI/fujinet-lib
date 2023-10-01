        .export     _fn_error

        .import     _fn_device_error
        .import     return0
        .import     return1

        .include    "device.inc"

; uint8_t fn_error(uint8_t code)
;
; Converts DCB status code into fujinet-network error.
; Keeps original status in _fn_device_error.
; Needs work if more than FN_ERR_OK/FN_ERR_IO_ERROR required

.proc _fn_error
        sta     _fn_device_error
        and     #$80
        bne     @error
        jmp     return0                 ; FN_ERR_OK by design
@error:
        jmp     return1                 ; FN_ERR_IO_ERROR by design

.endproc