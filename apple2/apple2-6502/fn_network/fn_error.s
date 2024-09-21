        .export     _fn_error

        .import     _fn_device_error
        .import     return0

        .include    "fujinet-network.inc"
        .include    "sp.inc"

; uint8_t fn_error(uint8_t code)
;
; convert SP error codes to standard errors for libray
.proc _fn_error
        sta     _fn_device_error

        ldx     #$00                ; high byte
        cmp     #SP_ERR_OK
        bne     not_ok
        txa                         ; FN_ERR_OK
        rts

; moved above so branch can reach
is_bad_cmd:
        lda     #FN_ERR_BAD_CMD
        rts

not_ok:
        cmp     #SP_ERR_BAD_CMD
        beq     is_bad_cmd
        cmp     #SP_ERR_BAD_PCNT
        beq     is_bad_cmd
        cmp     #SP_ERR_BAD_UNIT
        beq     is_bad_cmd
        cmp     #SP_ERR_BAD_CTRL
        beq     is_bad_cmd
        cmp     #SP_ERR_BAD_CTRL_PARM
        beq     is_bad_cmd
        cmp     #SP_ERR_BUS_ERR
        beq     is_io_error
        cmp     #SP_ERR_IO_ERROR
        beq     is_io_error
        cmp     #SP_ERR_NO_DRIVE
        beq     is_io_error
        cmp     #SP_ERR_NO_WRITE
        beq     is_io_error
        cmp     #SP_ERR_BAD_BLOCK
        beq     is_io_error
        cmp     #SP_ERR_DISK_SW
        beq     is_io_error
        cmp     #SP_ERR_DEV_SPEC0
        beq     is_io_error
        cmp     #SP_ERR_DEV_SPECF
        beq     is_io_error
        cmp     #SP_ERR_NON_FATAL50
        beq     is_warning
        cmp     #SP_ERR_NON_FATAL7F
        beq     is_warning
        cmp     #SP_ERR_OFFLINE
        beq     is_offline

is_unknown:
        ; didn't handle the code
        lda     #FN_ERR_UNKNOWN
        rts

is_io_error:
        lda     #FN_ERR_IO_ERROR
        rts

is_offline:
        lda     #FN_ERR_OFFLINE
        rts

is_warning:
        lda     #FN_ERR_WARNING
        rts

.endproc