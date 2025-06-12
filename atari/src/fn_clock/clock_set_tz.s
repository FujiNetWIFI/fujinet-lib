        .export         _clock_set_apetime_tz
        .export         _clock_set_tz

        .import         _bus
        .import         _fuji_success
        .import         _strlen

        .import         popax

        .include        "device.inc"
        .include        "fujinet-clock.inc"
        .include        "macros.inc"
        .include        "zp.inc"


; uint8_t clock_set_alternate_tz(char *tz);
_clock_set_alternate_tz:
        ; for backwards compatibility, this is the same as the old "APETIME" settz
        ldy     #SIO_APETIMECMD_SETTZ
        sty     IO_DCB::dcomnd
        bne     _clock_set_tz_common

; uint8_t clock_set_tz(char *tz);
_clock_set_tz:
        ldy     #'t'
        sty     IO_DCB::dcomnd
        ; ... fallthrough

_clock_set_tz_common:
        axinto  IO_DCB::dbuflo          ; the TZ string to send, will be null terminated
        jsr     _strlen

        ; add 1 for the null terminator
        clc
        adc     #$01
        bcc     :+
        inx
:       axinto  IO_DCB::dbytlo
        axinto  IO_DCB::daux1

; the SIO clock device follows the APETIME device ID:
; #define SIO_DEVICEID_APETIME 0x45

        mva     #SIO_CLOCK_DEVICE_ID, IO_DCB::ddevic
        mva     #$80, IO_DCB::dstats    ; write mode for atari
        ldx     #$00
        stx     IO_DCB::dunuse
        inx                             ; x = 1
        stx     IO_DCB::dunit
        inx                             ; x = 2
        stx     IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success
