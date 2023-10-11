        .export     _main
        .export     t_cb

        .export     t_devicespec
        .export     t_buffer
        .export     t_len
        .export     t_return_code

        .export     t_cb_executed
        .export     _network_status

        .import     _fn_network_bw
        .import     _fn_network_conn
        .import     _fn_network_error
        .import     _network_read
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
        jmp     _network_read
.endproc


; the callback for the network call
; we will be called twice, as test is copying >512 bytes, so
; set appropriate values in sp_payload for each time we're called, which we
; can detect in our count variable t_cb_executed
.proc t_cb
        inc     t_cb_executed
        lda     t_cb_executed
        cmp     #$01
        beq     r1
        cmp     #$02
        bne     r2
        cmp     #$03
        bne     r3

        ; round 1 - status
r1:
        mwa     t_len, _sp_payload

        ldx     #$00
        lda     t_return_code
        rts

        ; round 2 READ $69
r2:
        mwa     #$200, _sp_payload
        mva     #$69, _sp_payload+2

        ldx     #$00
        lda     t_return_code
        rts

        ; round 2 READ $69
r3:
        mwa     #$1, _sp_payload
        mva     #$96, _sp_payload+2

        ldx     #$00
        lda     t_return_code
        rts

.endproc

.proc _network_status
        mwa     #513, _fn_network_bw
        mva     #$00, _fn_network_conn
        sta     _fn_network_error

        rts
.endproc

.bss
t_devicespec:   .res 2
t_buffer:       .res 2
t_len:          .res 2

.data
t_cb_executed:  .byte 0
t_return_code:  .byte 0
