.export _main
.export t0
.export t0_end
.export t1
.export t1_end
.export tz0
.export tz1
.export bus_called

.export _bus
.export _fn_error

.import _clock_set_tz
.import _clock_set_alternate_tz

.include "macros.inc"
.include "zp.inc"

.code
_main:

t0:
        setax   #tz0
        jsr     _clock_set_tz
t0_end:

        dec     bus_called
t1:
        setax   #tz1
        jsr     _clock_set_alternate_tz
t1_end:

        rts

; mocks
_bus:
        mva     #$01, bus_called
        lda     #$00
        rts

_fn_error:
        rts

.data
bus_called:     .byte 0

.rodata

; data sent to FN
tz0:            .byte "UTC+1",0
tz1:            .byte "UTC+11",0
