.export _main
.export t0
.export t0_end
.export t1
.export t1_end
.export tz0
.export tz1
.export sp_control_called
.export spc_arg1
.export spc_arg2

.export _fn_error
.export _sp_control
.export _sp_get_clock_id
.export _sp_payload
.export _sp_count

.import _clock_set_tz
.import _clock_set_alternate_tz

.import popa

.include "macros.inc"
.include "zp.inc"

.code
_main:

t0:
        setax   #tz0
        jsr     _clock_set_tz
t0_end:

        dec     sp_control_called
t1:
        setax   #tz1
        jsr     _clock_set_alternate_tz
t1_end:

        rts

; mocks
_sp_get_clock_id:
        lda     #$01
        rts

_sp_control:
        ; copy the params into testable fields. Using C convention of left to right for the count
        sta     spc_arg2
        popa    spc_arg1
        inc     sp_control_called
        rts

_fn_error:
        rts

.bss

spc_arg1:       .res 1
spc_arg2:       .res 1

_sp_count:      .res 1
_sp_payload:    .res $20

.data
sp_control_called:      .byte 0

.rodata
; data sent to FN
tz0:            .byte "UTC+1",0
tz1:            .byte "UTC+2",0