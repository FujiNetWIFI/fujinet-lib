        .export         _clock_get_tz

        .import         _sp_count
        .import         _sp_get_clock_id
        .import         _sp_payload
        .import         _sp_status

        .import         pusha
        .import         return0
        .import         return1

        .include        "macros.inc"
        .include        "zp.inc"

; uint8_t clock_get_tz(uint8_t* tz);

_clock_get_tz:
        axinto  tmp_tz_loc
        jsr     _sp_get_clock_id
        bne     got_id

error:
        jmp     return1

got_id:
        ; sp_status(clock_id, 'G')
        jsr     pusha
        lda     #'G'
        jsr     _sp_status
        bne     error

        ; copy sp_count bytes from payload into buffer, there's always at least 1 byte (null terminator)
        mwa     tmp_tz_loc, ptr1
        ldy     #$00
:       lda     _sp_payload, y
        sta     (ptr1), y
        iny
        cpy     _sp_count
        bne     :-

        jmp     return0


.bss
tmp_tz_loc:     .res 2
