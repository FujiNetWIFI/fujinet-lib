        .export     _sp_init
        .export     find_slot

        .import     _sp_find_fuji
        .import     _sp_dispatch
        .import     return0
        .import     return1

        .include    "macros.inc"
        .include    "zp.inc"

; bool sp_init()
;
; returns true if Smart Port initialised
; otherwise false.
; sets 
.proc _sp_init
        jsr     find_slot
        bne     :+

        ; A/X are 0, so return that, as we didn't find a slot
        rts

        ; Do we really need to do this? it doesn't alter init in any way.
        ; I could imagine that we should try other Smart Ports (if there are any) checking each for a FujiNet device
        ; but the original implementation of this doesn't use the lack of FN to check other SP instances.

;         ; find FUJINET_DISK_0 device, and return 0/1 depending on if we found it.
; :       jsr     _sp_find_fuji
;         bne     found
;         ; A/X already 0
;         rts

; found:
        ; A is currently slot index, but need to convert to boolean
 :      jmp     return1

.endproc

; find a device slot that has Smart Port (1-7)
.proc find_slot
        mwa     #$c000, ptr1
        ldx     #$01

all_slots:
        inc     ptr1+1

        ; test bytes
        ldy     #$01
        lda     (ptr1), y
        cmp     #$20
        bne     :+
        ldy     #$03
        lda     (ptr1), y
        cmp     #$00
        bne     :+
        ldy     #$05
        lda     (ptr1), y
        cmp     #$03
        bne     :+
        ldy     #$07
        lda     (ptr1), y
        cmp     #$00
        beq     found_sp
:       inx
        cpx     #$08
        bne     all_slots

        ; not found, return 0
        jmp     return0

found_sp:
        ; set _sp_dispatch address while we have the correct slot in ptr1
        ldy     #$ff
        lda     (ptr1), y       ; dispatch vector is this value +3
        clc
        adc     #$03
        adw1    ptr1, a         ; move ptr1 to dispatch address, and store it
        mwa     ptr1, _sp_dispatch

        ; now return the slot id in A, with X = 0
        txa                     ; device index into a
        tay                     ; temp move into y, so that we can make setting of a last statement
        ldx     #$00            ; high byte of return
        tya                     ; low byte of return (and sets Z)
        rts
.endproc