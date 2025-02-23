INCLUDE "becker_pmd85.inc"

SECTION code_user

PUBLIC _dwread

; extern uint8_t __CALLEE__  dwread(uint8_t *buf, uint16_t count);

_dwread:
    pop     hl      ; return address
    pop     de      ; count
    ex      (sp), hl ; buffer
; HL = starting address where data is to be stored
; DE = number of bytes expected
dwread_asm:
    ld b,e          ; Number of loops is in DE
    dec de          ; Calculate DB value (destroys B, D and E)
    inc d
loop1:
    in BECKERSTATUS
    and 2
    jz loop1
    in BECKERDATA
    ld (hl),a
    ; next byte
    inc hl
    djnz loop1
    dec d
    jp nz,loop1
    ld l,1
    ret
