        .export     _sp_find_device
        .export     check_name

        .import     _sp_is_init
        .import     _sp_init
        .import     _sp_payload
        .import     _sp_status
        .import     incsp2
        .import     popax
        .import     pusha
        .import     pushax
        .import     return0

        .include    "macros.inc"
        .include    "zp.inc"

; int8_t sp_find_device(char *device_name);
;
; find the device number for the one matching given name
; return:
; errors are returned as negative values from the sp_status DIB call
; 0 = no errors, but no device found
; n = slot of given named device

.proc _sp_find_device
        axinto  ptr1            ; the device name we're looking for

        lda     _sp_is_init
        bne     do_find         ; already initialised, skip init

        ; we haven't initialized, so save ptr1, go do the init, then restore it
        lda     ptr1            ; x still has ptr1+1, just get a back
        jsr     pushax          ; store the values so we can run init
        jsr     _sp_init
        beq     not_found
        bpl     ok              ; succeeded if >0

        ; error initialising (-ve), or no network device found (0)
        ; just return the value, and let caller deal with it
not_found:
        jsr     incsp2          ; remove the device name ptr from stack, doesn't affect A
        cmp     #$00            ; set the N/Z flags according to value in A for the return
        rts

ok:
        popax   ptr1            ; restore the device name

do_find:
        ; find the device count by sending 0/0 status request
        pusha   #$00            ; doubles up as both parameters
        jsr     _sp_status
        beq     :+
        
        ; convert to an error code by making negative
        eor     #$ff
        clc
        adc     #$01
        rts

:       lda     _sp_payload     ; number of devices in payload[0], 1 based
        beq     exit            ; no devices found, don't request any information.

        sta     tmp1            ; device index, set to max (e.g. 6) initially

        mva     #$01, tmp2      ; this will be our device index to check

        ; get DIB for each device in turn looking for name
        lda     tmp2
:       jsr     pusha
        lda     #$03
        jsr     _sp_status
        bne     skip_next

        jsr     check_name
        beq     skip_next

        ; we found the device in slot tmp2
        ldx     #$00
        lda     tmp2
        rts

skip_next:
        ; keep looping until tmp2 (device index) is greater than device count
        inc     tmp2
        lda     tmp2
        cmp     tmp1
        bcc     :-
        beq     :-

exit:
        ; didn't find device
        jmp     return0

.endproc

; ptr1 is pointer to device name we are comparing to.
.proc check_name
        ldx     #$00                    ; count of matching chars, and string length in match
        ldy     #$00

:       lda     (ptr1), y               ; device name searching for
        beq     end_string
        cmp     _sp_payload+5, y       ; payload[5+i]th character
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