; void wait_jiffies(uint16_t jiffies)
; No timer on every model, so count cycles. One jiffy is padded to
; 17,030 cycles (262 lines x 65 cycles), the NTSC frame length.

        .export _wait_jiffies
        .importzp ptr1

_wait_jiffies:
        sta     ptr1
        stx     ptr1+1
        ora     ptr1+1          ; zero only if both bytes are zero
        beq     done
jiffy:  ldy     #13
outer:  ldx     #0
inner:  dex
        bne     inner
        dey
        bne     outer
        ldx     #58             ; pad: 2 + 58*5 - 1 = 291 cycles
pad:    dex
        bne     pad
        lda     ptr1            ; 16-bit decrement
        bne     :+
        dec     ptr1+1
:       dec     ptr1
        lda     ptr1
        ora     ptr1+1
        bne     jiffy
done:   rts
