; test adw macro
    .include    "macros.inc"

    .export test_adw_word_immw
    .export test_adw_word_word
    .export test_adw_word_immw_word
    .export test_adw_word_word_word
    .export test_adw_word_imm_no_c
    .export test_adw_word_imm_c
    .export test_adw_word_imm_word_no_c
    .export test_adw_word_imm_word_c


    .export t_t1, t_t2, t_t3, t_t4, t_t5, t_t6, t_t7, t_t8
    .export t_w1, t_w2, t_w3

.code

test_adw_word_immw:
    mwa t_w1, t_t1
    adw t_t1, #$1111        ; #$1234 + #$1111 = #$2345
    rts

test_adw_word_word:
    ; address values
    mwa t_w1, t_t2
    adw t_t2, t_w2          ; #$1234 + #$a013 = #$b247
    rts

test_adw_word_immw_word:
    adw t_w1, #$2222, t_t3  ; #$1234 + #$2222 = #$3456
    rts

test_adw_word_word_word:
    adw t_w2, t_w3, t_t4    ; #$a013 + #$1111 = #$b124
    rts

test_adw_word_imm_no_c:
    mwa t_w1, t_t5
    adw t_t5, #$01    ; #$1234 + #$01 = #$1235
    rts

test_adw_word_imm_c:
    mwa t_w1, t_t6
    adw t_t6, #$cd    ; #$1234 + #$cd = #$1301
    rts

test_adw_word_imm_word_no_c:
    adw t_w1, #$01, t_t7    ; #$1234 + #$01 = #$1235
    rts

test_adw_word_imm_word_c:
    adw t_w1, #$cd, t_t8    ; #$1234 + #$cd = #$1301
    rts

; input data from memory locations
t_w1:   .word $1234
t_w2:   .word $a013
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
