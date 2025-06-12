        .export         _clock_get_time

        .import         _sp_count
        .import         _sp_get_clock_id
        .import         _sp_payload
        .import         _sp_status

        .import         incsp2
        .import         popax
        .import         pusha
        .import         return0
        .import         return1

        .include        "macros.inc"
        .include        "zp.inc"

; uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);

_clock_get_time:
        sta     time_format             ; format, save it where we need it! saves a BSS byte

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
        sta     tmp1                    ; save the clock device id

        ; convert the time format to the appropriate device specific code.
        ; SIMPLE_BINARY     (0) -> 'T'
        ; PRODOS_BINARY     (1) -> 'P'
        ; APETIME_TZ_BINARY (2) -> 'A'
        ; APETIME_BINARY    (3) -> 'B'
        ; TZ_ISO_STRING     (4) -> 'S'
        ; UTC_ISO_STRING    (5) -> 'Z'

        ldx     #$00
time_format     = *-1
        ; ensure the value is valid
        cpx     #$06
        bcs     error
        jsr     pusha
        lda     code_table, x

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

        jmp     return0

.data
code_table:
        .byte 'T', 'P', 'A', 'B', 'S', 'Z'

