.export _main
.export t0
.export t0_end
.export output_buffer
.export set_alt_tz_called
.export alt_tz_called_with_ptr
.export tz0

.export _sp_status
.export _sp_count
.export _sp_payload
.export _sp_get_clock_id

.export _clock_set_alternate_tz
.export _fuji_success

.import _clock_get_time_tz

.import pushax

.include "zp.inc"
.include "macros.inc"

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
_sp_get_clock_id:
        lda     #$01
        rts

_sp_status:
        ; put the appropriate clock data into _sp_payload
        mwa     clock_ptr, ptr1
        mwa     #_sp_payload, ptr2
        ; decrease ptr2 by 1 to adjust for the fact the first byte of the clock data is the count, so y is too large by 1 to act as saving index
        sbw1    ptr2, #$01

        ldy     #$00
        lda     (ptr1), y               ; get the count of bytes. it's the first byte of the array from clock0...5
        tax
        iny
:       mva     {(ptr1), y}, {(ptr2), y}
        iny
        dex
        bne     :-

        ; add an extra 00 so we can detect we didn't run into next clock data in our buffer
        lda     #$00
        sta     (ptr2), y

        ; set the count
        dey
        sty     _sp_count
        lda     #$00
        rts

_fuji_success:
        rts

_clock_set_alternate_tz:
        axinto  alt_tz_called_with_ptr
        mva     #$01, set_alt_tz_called
        lda     #$00    ; FN_ERR_OK
        rts

.bss

_sp_count:      .res 1
clock_ptr:      .res 2

; for convenience, just make them same address, tests all use output_buffer
_sp_payload:
output_buffer:  .res 26

.data
set_alt_tz_called:      .byte 0
alt_tz_called_with_ptr: .word 0

.rodata
; data sent to FN
tz0:            .byte "UTC+1",0

; make these pascal style, length in first byte.
; this is return data from mock FN
clock0:         .byte $07, $00, $01, $02, $03, $04, $05, $06
