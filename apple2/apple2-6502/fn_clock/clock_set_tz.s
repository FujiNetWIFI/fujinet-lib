        .export         _clock_set_tz
        .export         _clock_set_alternate_tz

        .import         _fn_error
        .import         _memcpy
        .import         _sp_control
        .import         _sp_get_clock_id
        .import         _sp_payload
        .import         _strlen

        .import         incsp2
        .import         pusha
        .import         pushax
        .import         return1

        .include        "macros.inc"
        .include        "zp.inc"


; uint8_t clock_set_alternate_tz(char *tz);
; same as clock_set_tz, but sends 't' ctrl code
_clock_set_alternate_tz:
        ldy     #'t'
        sty     ctrl_code
        bne     _clock_set_tz_common    ; always

; uint8_t clock_set_tz(char *tz);
; sends 'T' ctrl code
_clock_set_tz:
        ldy     #'T'
        sty     ctrl_code
        ; fall through

_clock_set_tz_common:
        axinto  tmp_tz_ptr              ; save the tz

        ; get the device id of the clock, this is stored in _sp_clock_id, but also returned so we can check if it failed (0 is error)
        jsr     _sp_get_clock_id
        bne     got_id

        ; no clock found, return 1 as an error
        jmp     return1

got_id:
        jsr     pusha                   ; store the destination device for call to sp_control

        ; copy timezone string into sp_payload, including the null terminator
        pushax  #(_sp_payload + 2)      ; push destination for memcpy
        pushax  tmp_tz_ptr              ; push src for memcpy AND set A/X for strlen - doubling down!

        jsr     _strlen

        ; increment for the null terminator of the string
        clc
        adc     #$01
        bcc     :+
        inx

        ; store the length for sp_control call, and A/X is correctly set for memcpy
:       sta     _sp_payload + 0
        stx     _sp_payload + 1
        jsr     _memcpy

        ; src/dst were both stored on s/w stack already
        ; This is written to by the appropriate function, either 'T' for system TZ or 't' for alternate TZ
        lda     #$00
ctrl_code = *-1
        jsr     _sp_control

        ; convert to fujinet ok/error code
        jmp     _fn_error

.bss
tmp_tz_ptr:     .res 2