; test mva macro
    .include    "zp.inc"
    .include    "macros.inc"

    .export test_mway
    .export t_t1, t_t2, t_t3

.code

test_mway:

    ; causes values to go into t_t1/2/3
    ldy     #$00

    mwa     #t_t1, ptr1
    ; immediate values
    mway    #$1234, {(ptr1), y}
    
    ; absolute values from memory
    ldy     #$02
    mway    t_w1, {(ptr1), y}

    ; address of t_t1 in t_t3
    ldy     #$04
    mway    #t_t1, {(ptr1), y}

    rts

; input data from memory locations
t_w1:   .word $abcd

; target address for writing to that will be read in test
t_t1:   .word $0000
t_t2:   .word $0000
t_t3:   .word $0000
