; void wait_jiffies(uint8_t jiffies)
; No timer on every model, so count cycles: 13 x 1286 is about 1/60 s at 1.023 MHz.

        .export _wait_jiffies

_wait_jiffies:
        cmp     #0
        beq     done
jiffy:  ldy     #13
outer:  ldx     #0
inner:  dex
        bne     inner
        dey
        bne     outer
        sec
        sbc     #1
        bne     jiffy
done:   rts
