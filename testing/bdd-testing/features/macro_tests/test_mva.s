; test mva macro
    .include    "macros.inc"

    .export test_mva
    .export t_t1, t_t2, t_t3, t_t4, t_t5, t_t6, t_t7, t_t8, t_t9, t_t10

.code

test_mva:    
    ; immediate to address
    mva #$80, $80
    mva #$81, $2000

    ; address to address
    mva t_b1, $81
    mva t_b2, $2001

    ; address to {absolute, REG}
    ldx #$02
    mva t_b3, {$2000, x}

    ldy #$03
    mva t_b4, {$2000, y}

    ; {absolute, REG} to address. Use writes from above to copy into targets
    ldx #$02
    mva {$2000, x}, t_t1

    ldy #$03
    mva {$2000, y}, t_t2

    ; address to {(ZP), y}
    lda #<t_t3
    sta $90
    lda #>t_t3
    sta $91
    ldy #$00
    mva t_b5, {($90), y}
    ldy #$01
    mva t_b6, {($90), y}    ; writes to t_t4

    ; address to {(ZP, x)}
    lda #<t_t5
    sta $90
    lda #>t_t5
    sta $91
    ldx #0
    mva t_b7, {($90, x)}

    lda #<t_t6
    sta $92
    lda #>t_t6
    sta $93
    ldx #$02
    mva t_b8, {($90, x)}

    ; {(ZP), y} to address
    lda #<t_b9
    sta $90
    lda #>t_b9
    sta $91
    ldy #$00
    mva {($90), y}, t_t7    ; stores t_b9 in t_t7
    ldy #$01
    mva {($90), y}, t_t8    ; stores t_b10 in t_t8

    ; {(ZP, x)} to address
    lda #<t_b11
    sta $90
    lda #>t_b11
    sta $91
    ldx #0
    mva {($90, x)}, t_t9    ; stores t_b11 in t_t9

    lda #<t_b12
    sta $92
    lda #>t_b12
    sta $93
    ldx #$02
    mva {($90, x)}, t_t10   ; stores t_b12 in t_t10

    rts

; input data from memory locations
t_b1:   .byte $01
t_b2:   .byte $02
t_b3:   .byte $03
t_b4:   .byte $04
t_b5:   .byte $05
t_b6:   .byte $06
t_b7:   .byte $07
t_b8:   .byte $08
t_b9:   .byte $09
t_b10:  .byte $0a
t_b11:  .byte $0b
t_b12:  .byte $0c

; target address for writing to that will be read in test
t_t1:   .byte $00
t_t2:   .byte $00
t_t3:   .byte $00
t_t4:   .byte $00
t_t5:   .byte $00
t_t6:   .byte $00
t_t7:   .byte $00
t_t8:   .byte $00
t_t9:   .byte $00
t_t10:  .byte $00
