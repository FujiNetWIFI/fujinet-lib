.export _main
.export t0
.export t0_end
.export output_buffer
.export set_alt_tz_called
.export alt_tz_called_with_ptr
.export tz0

.export _bus
.export _clock_set_alternate_tz
.export _fuji_success

.import _clock_get_time_tz

.import pushax

.include "zp.inc"
.include "macros.inc"
.include "device.inc"

.code
_main:
        mva     #$00, set_alt_tz_called
        mwa     #clock0, clock_ptr
        pushax  #output_buffer
        pushax  #tz0
        lda     #0
t0:
        jsr     _clock_get_time_tz
t0_end:
        rts


; mocks
_bus:
        ; put the appropriate clock data into wherever IO_DCB::dbuf points to
        ; clock_ptr points to the correct return data
        mwa     clock_ptr, ptr1
        mwa     IO_DCB::dbuflo, ptr2

        ldy     #$00                  ; size to copy, e.g. 6
:       mva     {(ptr1), y}, {(ptr2), y}
        iny
        cpy     IO_DCB::dbytlo
        bne     :-

        ; add a zero to finish the buffer off
        lda     #$00
        sta     (ptr2), y
        rts

_fuji_success:
        rts

; capture what this was called with
_clock_set_alternate_tz:
        axinto  alt_tz_called_with_ptr
        mva     #$01, set_alt_tz_called
        lda     #$00
        rts

.bss

clock_ptr:      .res 2
output_buffer:  .res 31

.data
set_alt_tz_called:      .byte 0
alt_tz_called_with_ptr: .word 0

.rodata
; data sent to FN
tz0:            .byte "UTC+1",0

; data returned from FN
clock0:         .byte $00, $01, $02, $03, $04, $05, $06