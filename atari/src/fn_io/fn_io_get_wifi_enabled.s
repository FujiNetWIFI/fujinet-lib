        .export         _fn_io_get_wifi_enabled
        .import         fn_io_copy_cmd_data, _fn_io_do_bus
        .import         return0, return1

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "fn_data.inc"

; int fn_io_get_wifi_enabled()
;
; sets A=1 if wifi is enabled. 0 otherwise, X=0 in both cases for calling convention
.proc _fn_io_get_wifi_enabled
        setax   #t_io_get_wifi_enabled
        jsr     fn_io_copy_cmd_data
        mwa     #tmp9, IO_DCB::dbuflo
        jsr     _fn_io_do_bus

        ; was it set?
        lda     tmp9
        cmp     #$01
        bne     :+

        ; yes
        jmp     return1

        ; no
:       jmp     return0

.endproc

.rodata
t_io_get_wifi_enabled:
        .byte $ea, $40, $01, $00, $00, $00
