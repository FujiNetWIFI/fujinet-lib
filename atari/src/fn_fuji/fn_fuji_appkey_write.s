        .export         _fn_fuji_appkey_write

        .import         _bus
        .import         _fn_fuji_error
        .import         copy_fuji_cmd_data
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"
        .include        "fujinet-fuji.inc"

; uint8_t fn_fuji_appkey_write(uint16_t count, AppKeyWrite *buffer);
;
.proc _fn_fuji_appkey_write
        axinto  tmp7                    ; save buffer address

        setax   #t_fn_fuji_write_appkey
        jsr     copy_fuji_cmd_data

        mwa     tmp7, IO_DCB::dbuflo
        jsr     popax                   ; count of bytes to store in key file from buffer.
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2

        jsr     _bus
        jmp     _fn_fuji_error
.endproc

.rodata

.define AKWsz .sizeof(AppKeyWrite)

t_fn_fuji_write_appkey:
        .byte $de, $80, AKWsz, 0, $ff, $ff
