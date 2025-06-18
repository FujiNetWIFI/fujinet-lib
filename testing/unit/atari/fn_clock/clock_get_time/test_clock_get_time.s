.export _main
.export t0
.export t0_end
.export t1
.export t1_end
.export t2
.export t2_end
.export t3
.export t3_end
.export t4
.export t4_end
.export t5
.export t5_end
.export t6
.export t6_end
.export t7
.export t7_end
.export output_buffer
.export dstats_called_with
.export return_err

.export _bus
.export _clock_set_alternate_tz
.export _fn_device_error

.import _fn_error
.import _clock_get_time

.import pushax

.include "zp.inc"
.include "macros.inc"
.include "device.inc"

.code
_main:

        mva     #$01, return_err
        mwa     #clock0, clock_ptr
        pushax  #output_buffer
        lda     #0
t0:
        jsr     _clock_get_time
t0_end:

        mwa     #clock1, clock_ptr
        pushax  #output_buffer
        lda     #1
t1:
        jsr     _clock_get_time
t1_end:

        mwa     #clock2, clock_ptr
        pushax  #output_buffer
        lda     #2
t2:
        jsr     _clock_get_time
t2_end:

        mwa     #clock3, clock_ptr
        pushax  #output_buffer
        lda     #3
t3:
        jsr     _clock_get_time
t3_end:

        mwa     #clock4, clock_ptr
        pushax  #output_buffer
        lda     #4
t4:
        jsr     _clock_get_time
t4_end:

        mwa     #clock5, clock_ptr
        pushax  #output_buffer
        lda     #5
t5:
        jsr     _clock_get_time
t5_end:

        lda     #$ff              ; invalid time format, should be 0-6
t6:
        jsr     _clock_get_time
t6_end:

        ; emulate a bus error
        mva     #$80, return_err
        lda     #$00
t7:
        jsr     _clock_get_time
t7_end:



        rts

; mocks
_bus:
        ; put the appropriate clock data into wherever IO_DCB::dbuf points to
        ; clock_ptr points to the correct return data
        mwa     clock_ptr, ptr1
        mwa     IO_DCB::dbuflo, ptr2
        mva     IO_DCB::dstats, dstats_called_with

        ldy     #$00                  ; size to copy, e.g. 6
:       mva     {(ptr1), y}, {(ptr2), y}
        iny
        cpy     IO_DCB::dbytlo
        bne     :-

        ; add a zero to finish the buffer off
        lda     #$00
        sta     (ptr2), y

        ; return the custom error via dstats
        mva     return_err, IO_DCB::dstats
        rts

_clock_set_alternate_tz:
        rts

.bss

clock_ptr:              .res 2
output_buffer:          .res 31
_fn_device_error:       .res 1
dstats_called_with:     .res 1

.data
; return value for fn_error
return_err:     .byte 0

.rodata
clock0:         .byte $00, $01, $02, $03, $04, $05, $06
clock1:         .byte $10, $11, $12, $13
clock2:         .byte $20, $21, $22, $23, $24, $25
clock3:         .byte "YYYY-MM-DDTHH:MM:SS+HHMM", 0
clock4:         .byte "2025-06-11T20:19:00+0100", 0
clock5:         .byte "YYYYMMDD0HHMMSS000", 0
