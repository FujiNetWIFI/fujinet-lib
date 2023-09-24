; test adw1 macro
    .include    "macros.inc"

    .export test_adw1_word_imm
    .export test_adw1_word_imm_inc
    .export test_adw1_word_byte
    .export test_adw1_word_byte_inc

    .export t_t1, t_t2, t_t3, t_t4
    .export t_w1

.code

test_adw1_word_imm:
    mwa t_w1, t_t1
    adw1 t_t1, #$11         ; #$1234 + #$11 = #$1245
    rts

test_adw1_word_imm_inc:
    mwa t_w1, t_t2
    adw1 t_t2, #$f3         ; #$1234 + #$f3 = #$1327
    rts

test_adw1_word_byte:
    mwa t_w1, t_t3
    adw1 t_t3, t_b1         ; #$1234 + #$11 = #$1245
    rts

test_adw1_word_byte_inc:
    mwa t_w1, t_t4
    adw1 t_t4, t_b2         ; #$1234 + #$f3 = #$1327
    rts

; input data from memory locations
t_w1:   .word $1234

t_b1:   .byte $11
t_b2:   .byte $f3

; target address for writing to that will be read in test
t_t1:   .word $0000
t_t2:   .word $0000
t_t3:   .word $0000
t_t4:   .word $0000
