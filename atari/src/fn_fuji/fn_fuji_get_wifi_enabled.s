        .export         _fn_fuji_get_wifi_enabled
        .import         copy_fuji_cmd_data, _bus
        .import         return0, return1

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; int fn_fuji_get_wifi_enabled()
;
; sets A=1 if wifi is enabled. 0 otherwise, X=0 in both cases for calling convention
.proc _fn_fuji_get_wifi_enabled
        setax   #t_io_get_wifi_enabled
        jsr     copy_fuji_cmd_data
        mwa     #tmp9, IO_DCB::dbuflo
        jsr     _bus

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
