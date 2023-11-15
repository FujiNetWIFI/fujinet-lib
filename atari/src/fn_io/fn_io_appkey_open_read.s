        .export         _fn_io_appkey_open
        .export         _fn_io_appkey_read

        .import         _bus
        .import         _fn_io_error
        .import         copy_io_cmd_data
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"
        .include        "fujinet-io.inc"

; uint8_t fn_io_appkey_open(AppKeyOpen *buffer);
;
_fn_io_appkey_open:
        axinto  tmp7
        setax   #t_fn_io_open_appkey

ak_common:
        jsr     copy_io_cmd_data

        mwa     tmp7, IO_DCB::dbuflo

        jsr     _bus
        jmp     _fn_io_error


; uint8_t fn_io_appkey_read(AppKeyRead *buffer);
;
_fn_io_appkey_read:
        axinto  tmp7                    ; buffer location
        setax   #t_fn_io_read_appkey
        jmp     ak_common


.rodata

.define AKOsz .sizeof(AppKeyOpen)
.define AKRsz .sizeof(AppKeyRead)

t_fn_io_open_appkey:
        .byte $dc, $80, AKOsz, 0, 0, 0

t_fn_io_read_appkey:
        .byte $dd, $40, AKRsz, 0, 0, 0
