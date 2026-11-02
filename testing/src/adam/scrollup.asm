; Replaces z88dk's ansi_SCROLLUP, which clears the bottom row of the pattern
; table but not of the color table, so a colored cell leaves its color behind
    SECTION code_video_vdp
    PUBLIC  ansi_SCROLLUP

    EXTERN  __tms9918_scroll_buffer
    EXTERN  __tms9918_attribute
    EXTERN  CONSOLE_ROWS
    EXTERN  LDIRVM
    EXTERN  LDIRMV
    EXTERN  FILVRM

ansi_SCROLLUP:
    push    ix
    ld      b, CONSOLE_ROWS-1
    ld      hl, 256
scloop:
    push    bc
    push    hl
    call    row_up
    pop     hl
    push    hl
    ld      de, 8192
    add     hl, de
    call    row_up
    pop     hl
    inc     h
    pop     bc
    djnz    scloop
    dec     h
    push    hl
    xor     a
    ld      bc, 256
    call    FILVRM
    pop     hl
    ld      de, 8192
    add     hl, de
    ld      a, (__tms9918_attribute)
    ld      bc, 256
    call    FILVRM
    pop     ix
    ret

; Copy the 256-byte row at hl to the row above it
row_up:
    push    hl
    ld      de, __tms9918_scroll_buffer
    ld      bc, 256
    call    LDIRMV
    pop     hl
    ld      de, -256
    add     hl, de
    ex      de, hl
    ld      hl, __tms9918_scroll_buffer
    ld      bc, 256
    jp      LDIRVM
