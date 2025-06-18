        .export         _clock_get_time
        .export         _clock_get_time_tz

        .import         _bus
        .import         _fn_error
        .import         _clock_set_alternate_tz

        .import         incsp2
        .import         incsp4
        .import         popax
        .import         return0
        .import         return1

        .include        "clock.inc"
        .include        "device.inc"
        .include        "fujinet-network.inc"           ; for the FN_ERR codes
        .include        "macros.inc"
        .include        "zp.inc"


; uint8_t clock_get_time_tz(uint8_t* time_data, const char* tz, TimeFormat format);
_clock_get_time_tz:
        sta     time_format
        cmp     #TIMEFORMAT_COUNT      ; was the format value in range?
        bcc     valid_format

        jsr     incsp4
bad_cmd:
        ldx     #$00
        lda     #FN_ERR_BAD_CMD
        rts

valid_format:
        ; first read tz from string given, and set it as the alternate TZ in fujinet.
        ; use alternate FN timezone when calling FN
        mva     #$01, aux1_val
        jsr     popax
        jsr     _clock_set_alternate_tz
        beq     alternate_ok            ; returns success status

        ; an error, but we still have an arg on software stack
        jsr     incsp2
        jmp     return1         ; FN_ERR_IO_ERROR

alternate_ok:
        ldy     #$00
time_format = *-1
        bpl     common_tz               ; always

; uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);
_clock_get_time:
        tay                             ; time format enum
        ; use system FN timezone when calling FN
        mva     #$00, aux1_val
        cpy     #TIMEFORMAT_COUNT       ; was the format value in range?
        bcc     common_tz

        ; exit after removing params from stack
        jsr     incsp2
        jmp     bad_cmd

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
        stx     IO_DCB::daux2
        stx     IO_DCB::dbythi
        inx                            ; x = 1
        stx     IO_DCB::dunit
        inx                            ; x = 2 - as this is a FN non network call, we can keep this low
        stx     IO_DCB::dtimlo

        ; need to set daux1 to 1 if this is an alternate tz call
        ldx     #$00
aux1_val = *-1
        stx     IO_DCB::daux1

        jsr     _bus
        lda     IO_DCB::dstats
        jmp     _fn_error

.rodata

; tables for the commands that have to be sent for the different types of time command
; see fuji_clock.h
t_clock_get_time_cmd:
        .byte 'T', 'P', SIO_APETIMECMD_GETTIME, 'I', 'Z', 'S'

t_clock_get_time_len:
        .byte 7, 4, 6, 25, 25, 19
