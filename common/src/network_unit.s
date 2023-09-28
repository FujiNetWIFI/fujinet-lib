        .export     _network_unit

        .import     return1

        .include    "zp.inc"

; uint8_t network_unit(char *devicespec)
;
; Calculate the UNIT number from the device spec.
; e.g. "N:foo"  = 1
;      "N2:bar" = 2
; see testing/bdd-testing/features/atari/network_unit.feature

.proc _network_unit
        sta     tmp9            ; devicespec
        stx     tmp10

        ldy     #$01
        lda     (tmp9), y
        cmp     #':'
        bne     :+

        ; unit is 1, e.g. "N:"
        jmp     return1

:       iny
        lda     (tmp9), y
        cmp     #':'
        beq     @have_digit

        ; unit is 1, no ':' found in first 2 chars
        jmp     return1

@have_digit:
        ; e.g. "N1:"
        dey
        ldx     #$00                ; set high byte of return
        lda     (tmp9), y
        sec
        sbc     #$30                ; convert to integer from ascii
        rts

.endproc