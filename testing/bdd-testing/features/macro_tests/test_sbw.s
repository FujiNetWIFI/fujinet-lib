; test sbw macro
    .include    "macros.inc"

    .export test_sbw_word_immw
    .export test_sbw_word_word
    .export test_sbw_word_immw_word
    .export test_sbw_word_word_word
    .export test_sbw_word_imm_no_c
    .export test_sbw_word_imm_c
    .export test_sbw_word_imm_word_no_c
    .export test_sbw_word_imm_word_c


    .export t_t1, t_t2, t_t3, t_t4, t_t5, t_t6, t_t7, t_t8
    .export t_w1, t_w2, t_w3

.code

test_sbw_word_immw:
    mwa t_w1, t_t1
    sbw t_t1, #$1111        ; #$4321 - #$1111 = #$3210
    rts

test_sbw_word_word:
    ; address values
    mwa t_w1, t_t2
    sbw t_t2, t_w2          ; #$4321 - #$0fff = #$3322
    rts

test_sbw_word_immw_word:
    sbw t_w1, #$2222, t_t3  ; #$4321 - #$2222 = #$20ff
    rts

test_sbw_word_word_word:
    sbw t_w1, t_w2, t_t4    ; #$4321 - #$0fff = #$3322
    rts

test_sbw_word_imm_no_c:
    mwa t_w1, t_t5
    sbw t_t5, #$01          ; #$4321 - #$01 = #$4320
    rts

test_sbw_word_imm_c:
    mwa t_w1, t_t6
    sbw t_t6, #$cd          ; #$4321 - #$cd = #$4254
    rts

test_sbw_word_imm_word_no_c:
    sbw t_w1, #$01, t_t7    ; #$4321 - #$01 = #$4320
    rts

test_sbw_word_imm_word_c:
    sbw t_w1, #$cd, t_t8    ; #$4321 - #$cd = #$4254
    rts

; input data from memory locations
t_w1:   .word $4321
t_w2:   .word $0FFF
t_w3:   .word $1111

; target address for writing to that will be read in test
t_t1:   .word $0000
t_t2:   .word $0000
t_t3:   .word $0000
t_t4:   .word $0000
t_t5:   .word $0000
t_t6:   .word $0000
t_t7:   .word $0000
t_t8:   .word $0000
