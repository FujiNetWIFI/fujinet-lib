        .export     _main
        .export     t_cb

        .export     t_devicespec
        .export     t_buffer
        .export     t_len
        .export     t_return_code
        .export     t_r1b1
        .export     t_r1b2
        .export     t_r2b1

        .export     t_cb_executed

        .import     _network_write
        .import     _sp_init
        .import     _sp_payload
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
        jmp     _network_write
.endproc


; the callback for the network call
; we will be called twice, as test is copying >512 bytes, so
; set appropriate values in sp_payload for each time we're called, which we
; can detect in our count variable t_cb_executed
.proc t_cb
        inc     t_cb_executed
        lda     t_cb_executed
        cmp     #$01
        bne     r2


        ; round 1 copy _sp_payload to t_b1
r1:
        mva     _sp_payload, t_r1b1
        mva     _sp_payload+511, t_r1b2
        jmp     :+

        ; round 2, only 1 byte copied
r2:
        mva     _sp_payload, t_r2b1

:       ldx     #$00
        lda     t_return_code
        rts

.endproc

.bss
t_devicespec:   .res 2
t_buffer:       .res 2
t_len:          .res 2

t_r1b1:           .res 1
t_r1b2:           .res 1
t_r2b1:           .res 1

.data
t_cb_executed:  .byte 0
t_return_code:  .byte 0
