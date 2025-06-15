        .export         _clock_get_time
        .export         _clock_get_time_tz

        .import         _sp_count
        .import         _sp_get_clock_id
        .import         _sp_payload
        .import         _sp_status

        .import         _clock_set_alternate_tz

        .import         incsp2
        .import         incsp4
        .import         popax
        .import         pusha
        .import         return0
        .import         return1

        .include        "macros.inc"
        .include        "fujinet-network.inc"           ; for FN_ERR_* codes
        .include        "zp.inc"

; for apple2 we support 6 
TIMEFORMAT_COUNT                := $06


; uint8_t clock_get_time_tz(uint8_t* time_data, const char* tz, TimeFormat format);
_clock_get_time_tz:
        ldx     #$20                    ; convert to lower case
        stx     lower_mask

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
        jsr     popax
        jsr     _clock_set_alternate_tz

        beq     common_tz
        ; an error, but we still have an arg on software stack
        jsr     incsp2
        jmp     return1         ; FN_ERR_IO_ERROR


; uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);
_clock_get_time:
        ldx     #$00            ; don't alter the byte, keep upper case
        stx     lower_mask

        sta     time_format
        cmp     #TIMEFORMAT_COUNT       ; was the format value in range?
        bcc     common_tz

        ; exit after removing params from stack
        jsr     incsp2
        jmp     bad_cmd

common_tz:
        ; get the device id of the clock, this is stored in _sp_clock_id, but also returned so we can check if it failed (0 is error)
        jsr     _sp_get_clock_id
        bne     got_id

        ; no clock found, return 1 as an error (FN_ERR_IO_ERROR)
        ; but first adjust the stack to remove the data pointer
error:
        jsr     incsp2
        jmp     return1

got_id:
        ; call sp_status(uint8_t dest, uint8_t statcode)

        ; convert the time format to the appropriate device specific code.
        ; SIMPLE_BINARY     (0) -> 'T'
        ; PRODOS_BINARY     (1) -> 'P'
        ; APETIME_TZ_BINARY (2) -> 0x99
        ; APETIME_BINARY    (3) -> 0x93
        ; TZ_ISO_STRING     (4) -> 'I'
        ; UTC_ISO_STRING    (5) -> 'Z'
        ; APPLE3 SOS format (6) -> 'S'

        jsr     pusha                   ; A = destination device (clock id)
        ldx     #$00
time_format     = *-1
        lda     code_table, x
        ; convert to lower case if needed (set bit 0x20), caller sets the next byte to use
        ora     #$00
lower_mask = *-1

        jsr     _sp_status
        bne     error

        ; results are in sp_payload, the clock data returned is small (4 to 26 bytes)
        ; _sp_count holds the number of bytes to copy from sp_payload, will always contain at least 1 byte (null terminator)
        popax   ptr1                    ; read the time_data location into ptr1
        ldy     #$00
:       lda     _sp_payload, y
        sta     (ptr1), y
        iny
        cpy     _sp_count
        bne     :-

        jmp     return0         ; FN_ERR_OK

.data
code_table:
        .byte 'T', 'P', 'A', 'I', 'Z', 'S'
