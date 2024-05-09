        .export         _fuji_get_wifi_enabled

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         return0
        .import         return1

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; int fuji_get_wifi_enabled()
;
; sets A=1 if wifi is enabled. 0 otherwise, X=0 in both cases for calling convention
.proc _fuji_get_wifi_enabled
        setax   #t_fuji_get_wifi_enabled
        jsr     _copy_fuji_cmd_data
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
t_fuji_get_wifi_enabled:
        .byte $ea, $40, $01, $00, $00, $00
