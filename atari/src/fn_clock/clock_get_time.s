        .export         _clock_get_time

        .import         _bus
        .import         _fuji_success

        .import         incsp2
        .import         popax
        .import         return0

        .include        "device.inc"
        .include        "fujinet-clock.inc"
        .include        "macros.inc"
        .include        "zp.inc"

; uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);
_clock_get_time:
        tay
        cpy     #$06                    ; was the format value in range?
        bcc     ok

        ; return an error status
        jsr     incsp2                  ; remove stack parameter
        jmp     return0

ok:
        lda     t_clock_get_time_cmd, y
        sta     IO_DCB::dcomnd
        lda     t_clock_get_time_len, y
        sta     IO_DCB::dbytlo
        popax   IO_DCB::dbuflo

; the SIO clock device follows the APETIME device ID:
; #define SIO_DEVICEID_APETIME 0x45
        mva     #SIO_CLOCK_DEVICE_ID, IO_DCB::ddevic
        mva     #$40, IO_DCB::dstats
        ldx     #$00
        stx     IO_DCB::dunuse
        stx     IO_DCB::daux1
        stx     IO_DCB::daux2
        stx     IO_DCB::dbythi
        inx                            ; x = 1
        stx     IO_DCB::dunit
        inx                            ; x = 2 - as this is a FN non network call, we can keep this low
        stx     IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success

.rodata

; tables for the commands that have to be sent for the different types of time command
; see fuji_clock.h
t_clock_get_time_cmd:
        .byte 'T', 'P', SIO_APETIMECMD_GETTZTIME, SIO_APETIMECMD_GETTIME, 'S', 'Z'

t_clock_get_time_len:
        .byte 7, 4, 6, 6, 25, 25
