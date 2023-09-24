; test mva macro
    .include    "macros.inc"

    .export test_mwa
    .export t_t1, t_t2, t_t3

.code

test_mwa:    
    ; immediate values
    mwa #$1234, t_t1
    
    ; absolute values from memory
    mwa t_w1, t_t2

    ; address of t_t1 in t_t3
    mwa #t_t1, t_t3

    rts

; input data from memory locations
t_w1:   .word $abcd

; target address for writing to that will be read in test
t_t1:   .word $0000
t_t2:   .word $0000
t_t3:   .word $0000
