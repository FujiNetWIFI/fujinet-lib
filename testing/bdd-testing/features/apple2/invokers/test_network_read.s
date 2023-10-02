        .export     _main
        .export     t_devicespec
        .export     t_buffer
        .export     t_len
        .export     t_return_code

        .export     t_cb_executed

        .import     _network_read
        .import     _sp_init
        .import     pusha
        .import     pushax
        .import     return0
        .import     spe_cb

        .include    "macros.inc"

.proc _main
        ; setup callback
        mwa     #t_cb, spe_cb

        ; would have been called by open normally. in this test, we haven't done it yet
        jsr     _sp_init

        ; call function under test
        pushax  t_devicespec
        pushax  t_buffer
        setax   t_len
        jmp     _network_read
.endproc


; the callback for the network call
.proc t_cb
        inc     t_cb_executed

        ldx     #$00
        lda     t_return_code
        rts
.endproc

.bss
t_devicespec:   .res 2
t_buffer:       .res 2
t_len:          .res 2

.data
t_cb_executed:  .byte 0
t_return_code:  .byte 0
