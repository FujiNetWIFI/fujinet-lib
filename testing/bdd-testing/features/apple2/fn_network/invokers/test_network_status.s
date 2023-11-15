        .export     _main
        .export     t_cb

        .export     t_devicespec
        .export     t_bw
        .export     t_c
        .export     t_err
        .export     t_cb_executed
        .export     t_return_code
        .export     t_statcode

        .export     t_status_bw
        .export     t_status_conn
        .export     t_status_err

        .import     _network_status
        .import     _sp_init
        .import     _sp_payload
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
        pushax  t_bw
        pushax  t_c
        setax   t_err
        jmp     _network_status
.endproc

.proc t_cb
        ; the callback for the status call
        sta     t_statcode
        inc     t_cb_executed

        mwa     t_status_bw, _sp_payload
        mva     t_status_conn, _sp_payload+2
        mva     t_status_err, _sp_payload+3

        ldx     #$00
        lda     t_return_code
        rts
.endproc

.bss
t_devicespec:   .res 2
t_bw:           .res 2
t_c:            .res 2
t_err:          .res 2

; for testing CB was called with correct statcode
t_statcode:     .res 1

t_status_bw:   .res 2
t_status_conn: .res 1
t_status_err:  .res 1

.data
t_cb_executed:  .byte 0
t_return_code:  .byte 0
