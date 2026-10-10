INCLUDE "becker_pmd85.inc"

SECTION code_user

PUBLIC _dwwrite

; extern uint8_t __CALLEE__  dwwrite(const uint8_t *buf, uint16_t count);

_dwwrite:
    pop     hl      ; return address
    pop     de      ; count
    ex      (sp), hl ; buffer
; HL = starting address of data to send
; DE = number of bytes to send
dwwrite_asm:
    ld b,e          ; Number of loops is in DE
    dec de          ; Calculate DB value (destroys B, D and E)
    inc d
loop1:
    ld a,(hl)
    out BECKERDATA
    ; next byte
    inc hl
    djnz loop1
    dec d
    jp nz,loop1
    ld l,1
    ret
