; test ada macro
    .include    "macros.inc"

    .export test_ada
    .export t_t1

.code

test_ada:
    ada t_t1
    rts

; target address for writing to that will be read in test
t_t1:   .word $0000
