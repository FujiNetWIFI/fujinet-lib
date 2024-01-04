        .export     _sp_init
        .export     find_slot

        .import     _sp_dispatch_fn
        .import     _sp_find_fuji
        .import     _sp_is_init
        .import     incsp2
        .import     return0
        .import     return1

        .include    "macros.inc"
        .include    "zp.inc"

; bool sp_init();
;
; returns true if Smart Port initialised
; otherwise false.
.proc _sp_init
        mva     #$01, _sp_is_init
        jsr     find_slot
        bne     :+

        ; A/X are 0, so return that, as we didn't find a slot
        rts

 :      jmp     return1

.endproc

; find a device slot that has Smart Port with a NETWORK adapter
; going from 7 down to 1, which is more likely to hit a FN device before some other RAM Card etc.
.proc find_slot
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

        ; not found, return 0
        jmp     return0

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
        bne     @found_network

        ; didn't find network, keep trying more slots
        ; restore ptr1
        popax   ptr1
        ; restore the id we last tried into X
        pla
        tax

        ; loop for next device
        bne     @no_match

@found_network:
        ; now return the slot id in A, with X = 0
        jsr     incsp2          ; remove the ptr1 saved data from sw stack
        pla                     ; restore network slot number
        tay                     ; temp move into y, so that we can make setting of a last statement
        ldx     #$00            ; high byte of return
        tya                     ; low byte of return (and sets Z)
        rts
.endproc