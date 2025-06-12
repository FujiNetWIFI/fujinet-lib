        .export         _clock_get_time
        .export         _clock_get_time_tz

        .import         _bus
        .import         _fuji_success
        .import         _clock_set_alternate_tz

        .import         incsp2
        .import         popax
        .import         return0

        .include        "device.inc"
        .include        "fujinet-clock.inc"
        .include        "macros.inc"
        .include        "zp.inc"


; either 2 or 4 bytes to increase sp4 by
exit_bad_tz4:
        jsr     incsp2

exit_bad_tz2:
        jsr     incsp2
        jmp     return0


; uint8_t clock_get_time_tz(uint8_t* time_data, const char* tz, TimeFormat format);
_clock_get_time_tz:
        cmp     #$06                    ; was the format value in range?
        bcs     exit_bad_tz4            ; too large, exit and remove 2 pointers (4 bytes) from stack

        ; save it while we deal with the TZ string
        sta     tmp_format
        ; first read tz from string given, and set it as the alternate TZ in fujinet.
        jsr     popax
        jsr     _clock_set_alternate_tz

        ; an error, but we still have an arg on software stack
        bne     exit_bad_tz2

        ldy     tmp_format
        bpl     common_tz               ; it's between 0 and 6 validated, so this will always branch

; uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);
_clock_get_time:
        tay                             ; time format enum
        cpy     #$06                    ; was the format value in range?
        bcs     exit_bad_tz2

common_tz:
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

.bss
tmp_format:     .res 1

.rodata

; tables for the commands that have to be sent for the different types of time command
; see fuji_clock.h
t_clock_get_time_cmd:
        .byte 'T', 'P', SIO_APETIMECMD_GETTZTIME, SIO_APETIMECMD_GETTIME, 'S', 'Z'

t_clock_get_time_len:
        .byte 7, 4, 6, 6, 25, 25
