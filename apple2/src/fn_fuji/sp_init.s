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

; uint8_t sp_init();
;
; returns network slot if Smart Port initialised and has a NETWORK adapter
; otherwise 0.
.proc _sp_init
        mva     #$01, _sp_is_init
        mva     #$00, _sp_network

; find a device slot that has Smart Port with a NETWORK adapter
; going from 7 down to 1, which is more likely to hit a FN device before some other RAM Card etc.
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

        ; not found, return 0 in A/X
        ; first, clear the dispatch function in case it was set when testing for a network device
        ldx     #$00
        stx     _sp_dispatch_fn
        stx     _sp_dispatch_fn+1
        txa
        rts

@found_sp:
        ; set _sp_dispatch_fn address while we have the correct slot in ptr1
        ldy     #$ff
        lda     (ptr1), y       ; _sp_dispatch vector is this value +3
        clc
        adc     #$03
        adw1    ptr1, a         ; move ptr1 to _sp_dispatch address, and store it
        mwa     ptr1, _sp_dispatch_fn

        ; does this device have a NETWORK adapter?
        ; first save X which is the slot ID
        txa
        pha

        ; save ptr1 which is current slot address we're trying, it gets trashed by sp_find_network
        pushax  ptr1

        jsr     _sp_find_network
        bpl     @found_network

        ; didn't find network, keep trying more slots
        ; restore ptr1
        popax   ptr1
        ; restore the id we last tried into X
        pla
        tax

        ; loop for next device
        bne     @no_match

@found_network:
        ; a contains the found slot id of the network device
        sta     _sp_network     ; save the value for other functions to use

        ; fix the stack
        jsr     incsp2          ; remove the ptr1 we saved for looping
        pla                     ; remove the old X index we saved for looping
        
        ; now return the slot id in A, with X = 0
        ldx     #$00            ; high byte of return
        lda     _sp_network     ; low byte of return (and sets Z)
        rts
.endproc