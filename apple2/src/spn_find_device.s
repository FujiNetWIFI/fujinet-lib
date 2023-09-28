        .export     _spn_find_device
        .export     check_name

        .import     _spn_payload
        .import     _spn_status
        .import     pusha
        .import     return0

        .include    "macros.inc"
        .include    "zp.inc"

; int8_t spn_find_device(char *device_name);
;
; find the device number for the one matching given name
; return:
; errors are returned as negative values from the sp_status DIB call
; 0 = no errors, but no device found
; n = slot of given named device
.proc _spn_find_device
        axinto  tmp9                ; the device name we're looking for

        ; find the device count - do we need to keep doing this?
        pusha   #$00
        ; lda    #$00    ; already 0
        jsr     _spn_status
        beq     :+
        
        ; convert to an error code by making negative
        eor     #$ff
        clc
        adc     #$01
        rts

:       mwa     #_spn_payload, ptr1
        ldy     #$00
        lda     (ptr1), y       ; number of devices in payload[0], 1 based
        sta     tmp8            ; device index, set to max (e.g. 6) initially

        mva     #$01, tmp5      ; this will be our device index to check. saves time in emulator as fuji is device 1

        ; get DIB for each device in turn looking for name
:       pusha   tmp5
        lda     #$03
        jsr     _spn_status         ; trashes x! calls dispatch function, but tmpX shouldn't be touched

        bne     skip_next

        jsr     check_name
        beq     skip_next

        ; we found the device in slot tmp5
        ldx     #$00
        lda     tmp5
        rts

skip_next:
        ; keep looping until tmp5 (device index) is greater than device count
        inc     tmp5
        cmp     tmp8
        bcc     :-
        beq     :-

        ; didn't find device
        jmp     return0

.endproc

.proc check_name
        ; align ptr to string to y index of payload for the device name just fetched
        mwa     #_spn_payload, ptr1
        mwa     tmp9, tmp6      ; use tmp6/7 as the pointer to string looking for with Y adjustment so we don't change tmp9/10
        sbw1    tmp6, #$05
        ldx     #$00            ; count of matching chars, and string length in match
        ldy     #$05

:       lda     (tmp6), y       ; device name searching for
        beq     end_string
        cmp     (ptr1), y       ; payload[5+i]th character
        bne     not_found
        inx
        iny
        bne     :-

not_found:
        lda     #$00
        rts

end_string:
        ; if x > 0, we matched at least 1 char before hitting end of string, so it's a match
        ; simply move x into a and return, if it's 0 that's an error, otherwise it's strlen
        txa
        rts

.endproc