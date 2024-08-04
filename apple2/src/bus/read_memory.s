        .export         _read_memory

        .import         popa

        .include        "zp.inc"

; uint8_t read_memory(uint8_t offset, uint16_t address) {
;         return *(((volatile uint8_t *) address) + offset);
; }

.proc _read_memory
        sta     ptr1
        stx     ptr1+1

        jsr     popa
        tay
        ldx     #$00
        lda     (ptr1), y
        rts
.endproc
