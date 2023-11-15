        .export     _main
        .export     t_cb

        .export     t_devicespec
        .export     t_mode
        .export     t_translate

        .export     t_cb_a
        .export     t_cb_codes
        .export     t_cb_executed

        .export     t_r1_cmd
        .export     t_r1_unit
        .export     t_r1_error
        .export     t_r1_payload
        .export     t_r2_cmd
        .export     t_r2_unit
        .export     t_r2_error
        .export     t_r2_payload

        .import     _network_open
        .import     _sp_init
        .import     _sp_payload
        .import     pusha
        .import     pushax
        .import     spe_cb
        .import     spe_cmd
        .import     spe_dest
        .import     spe_code

        .include    "macros.inc"
        .include    "zp.inc"

.proc _main
        ; setup callback
        mwa     #t_cb, spe_cb

        ; call function under test
        pushax  t_devicespec
        pusha   t_mode
        lda     t_translate
        jmp     _network_open
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

        ; call 1 - sp_open
r1:
        ; save all the args
        mva     spe_cmd, t_r1_cmd
        mva     spe_dest, t_r1_unit
        setax   #t_r1_payload
        jsr     copy_payload

        ldx     #$00
        lda     t_r1_error
        rts

        ; call 2 - control "O"
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
t_mode:         .res 1
t_translate:    .res 1

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

t_r1_error:  .byte 0
t_r2_error:  .byte 0
