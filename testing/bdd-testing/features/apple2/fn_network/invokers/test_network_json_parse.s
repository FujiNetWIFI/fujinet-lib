        .export     _main
        .export     t_cb

        .export     t_devicespec

        .export     t_cb_a
        .export     t_cb_codes
        .export     t_cb_executed

        .export     t_sp_network

        .export     t_r1_cmd
        .export     t_r1_unit
        .export     t_r1_error
        .export     t_r1_payload
        .export     t_r2_cmd
        .export     t_r2_unit
        .export     t_r2_error
        .export     t_r2_payload

        .import     _network_json_parse
        .import     _sp_init
        .import     _sp_network
        .import     _sp_payload
        .import     pushax
        .import     return0
        .import     spe_cb
        .import     spe_cmd
        .import     spe_dest
        .import     spe_code

        .include    "macros.inc"
        .include    "zp.inc"

.proc _main
        ; setup callback
        mwa     #t_cb, spe_cb

        ; would have been called by open normally. in this test, we haven't done it yet
        jsr     _sp_init

        ; should we unset the sp_network value?
        lda     t_sp_network
        bne     :+              ; no

        mva     #$00, _sp_network

        ; call function under test
:       setax   t_devicespec
        jmp     _network_json_parse
.endproc


; the callback for the network call
; we can use the values in spe_* to determine parameters invoked that got us here, if needed
.proc t_cb
        ; store calling parameter in table for later inspection
        ldx     t_cb_executed
        sta     t_cb_a, x
        mva     spe_code, {t_cb_codes, x}

        inc     t_cb_executed
        lda     t_cb_executed
        cmp     #$01
        beq     r1
        cmp     #$02
        beq     r2
        ; this is a test! shouldn't happen unless implementation drastically changes
        bne     error

        ; call 1 - control: JSON Channel Mode
r1:
        ; save all the args
        mva     spe_cmd, t_r1_cmd
        mva     spe_dest, t_r1_unit
        setax   #t_r1_payload
        jsr     copy_payload

        ldx     #$00
        lda     t_r1_error
        rts

        ; call 2 - status (length)
r2:
        mva     spe_cmd, t_r2_cmd
        mva     spe_dest, t_r2_unit
        setax   #t_r2_payload
        jsr     copy_payload

        ldx     #$00
        lda     t_r2_error
        rts

error:
        lda     #$ff
        rts

copy_payload:
        ; a/x contains destination
        axinto  t_payload_copy_dst
        lda     tmp9
        pha
        lda     tmp10
        pha
        mwa     t_payload_copy_dst, tmp9
        ldy     #$00
:       mva     {_sp_payload, y}, {(tmp9), y}
        iny
        cpy     #20
        bne     :-
        pla
        sta     tmp10
        pla
        sta     tmp9
        rts
.endproc

.bss
t_devicespec:   .res 2
t_query:        .res 2
t_s:       .res 2

t_payload_copy_dst: .res 2

; save locations for accumulator, ctrlcode/statcode when cb invoked
t_cb_a:         .res 5
t_cb_codes:     .res 5

; save the calling parameters/values for testing in feature
t_r1_cmd:       .res 1
t_r1_unit:      .res 1
t_r1_payload:   .res 20

t_r2_cmd:       .res 1
t_r2_unit:      .res 1
t_r2_payload:   .res 20

.data

; some control variables to force error conditions, and track which call we're processing
t_cb_executed:  .byte 0

t_r1_error:     .byte 0
t_r2_error:     .byte 0

; used to force _sp_network to 0 after init, so we can test what happens when it's not set
; if it's not 0, it will not be changed. if it is 0, it will set the real _sp_network to 0
t_sp_network:   .byte 1