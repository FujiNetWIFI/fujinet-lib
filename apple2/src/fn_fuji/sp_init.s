        .export     _sp_init

        .import     _sp_dispatch_fn
        .import     _sp_find_fuji
        .import     _sp_is_init
        .import     _sp_find_network
        .import     _sp_network
        .import     incsp2
        .import     popax
        .import     pushax
        .import     return0
        .import     return1

        .include    "macros.inc"
        .include    "zp.inc"

; int8_t sp_init();
;
; returns network slot if Smart Port initialised and has a NETWORK adapter
; otherwise 0 if there were no errors but no SP device, else the -ve value of the device error from SP, as returned by sp_find_device.
.proc _sp_init
        mva     #$01, _sp_is_init               ; we assume you can only call init once. No devices are mounted after powerup
        mva     #$00, _sp_network
        sta     last_sp_error

; find a device slot that has Smart Port with a NETWORK adapter
; going from 7 down to 1, which is more likely to hit a FN device before some other RAM Card etc (thanks @ShunKita)
        mwa     #$c700, ptr1
        ldx     #$01

@all_slots:
        ; test Cn01, Cn03, Cn05, Cn07 match expected values.
        ldy     #$01
        lda     (ptr1), y
        cmp     #$20
        bne     @no_match
        ldy     #$03
        lda     (ptr1), y
        cmp     #$00
        bne     @no_match
        ldy     #$05
        lda     (ptr1), y
        cmp     #$03
        bne     @no_match
        ldy     #$07
        lda     (ptr1), y
        cmp     #$00
        beq     @found_sp

@no_match:
        dec     ptr1+1
        inx
        cpx     #$08
        bne     @all_slots

        ; not found, return value of last_sp_error, which will be -ve or 0 for the caller to know there was a device error or not
        ; first, clear the dispatch function in case it was set when testing for a network device
        ldx     #$00
        stx     _sp_dispatch_fn
        stx     _sp_dispatch_fn+1
        lda     last_sp_error
        rts

@found_sp:
        ; set _sp_dispatch_fn address while we have the correct slot in ptr1
        ; first copy it into ptr2, as we need to keep the original ptr1 for next loop if this card fails.
        mwa     ptr1, ptr2
        ldy     #$ff
        lda     (ptr2), y       ; _sp_dispatch vector is this value +3
        clc
        adc     #$03
        adw1    ptr2, a         ; move ptr1 to _sp_dispatch address, and store it
        mwa     ptr2, _sp_dispatch_fn

        ; does this device have a NETWORK adapter?
        ; first save X which is our counter for number of devices tested so far
        txa
        pha

        ; save ptr1 which is current slot address we're trying, it gets trashed by sp_find_network
        pushax  ptr1

        jsr     _sp_find_network
        bpl     @found_network
        beq     :+

        ; we had a -ve value, which indicates a real SP error of some kind, capture it
        ; so we can indicate the issue at the end if we didn't find a SP with network
        sta     last_sp_error

        ; as we didn't find network, keep trying more slots
        ; restore ptr1
:       popax   ptr1
        ; restore the id we last tried into X
        pla
        tax

        ; loop for next device
        bne     @no_match

@found_network:
        ; a contains the found slot id of the network device
        sta     _sp_network     ; save the value for other functions to use

        ; if one of the potentially other SP devices was in error, we will ignore it, as we found the right one with network.
        mva     #$00, last_sp_error

        ; fix the stack
        jsr     incsp2          ; remove the ptr1 we saved for looping
        pla                     ; remove the old X index we saved for looping
        
        ; now return the slot id in A, with X = 0
        ldx     #$00            ; high byte of return
        lda     _sp_network     ; low byte of return (and sets Z)
        rts
.endproc

.data
; store the last SP error we got.
; we want to ensure any error from the last SP device detected that we discard is captured.
last_sp_error: .byte 0
