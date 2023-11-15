        .export     _main
        .export     t_devicespec
        .export     t_cb
        .export     t_cb_executed

        .import     _network_close
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
        setax   t_devicespec
        jmp     _network_close
.endproc


.proc t_cb
        ; the callback for the network call
        inc     t_cb_executed

        ldx     #$00
        lda     t_return_code
        rts
.endproc

.bss
t_devicespec:   .res 2

.data
t_cb_executed:  .byte 0
t_return_code:  .byte 0
