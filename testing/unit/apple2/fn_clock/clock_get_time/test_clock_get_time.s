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
.export output_buffer

.export _sp_status
.export _sp_count
.export _sp_payload
.export _sp_get_clock_id

.export _clock_set_alternate_tz
.export _fuji_success

.import _clock_get_time

.import pushax

.include "zp.inc"
.include "macros.inc"

.code
_main:

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
        rts

.bss

_sp_count:      .res 1
clock_ptr:      .res 2

; for convenience, just make them same address, tests all use output_buffer
_sp_payload:
output_buffer:  .res 26

.rodata
; make these pascal style, length in first byte.
clock0:         .byte $07, $00, $01, $02, $03, $04, $05, $06
clock1:         .byte $04, $10, $11, $12, $13
clock2:         .byte $06, $20, $21, $22, $23, $24, $25
clock3:         .byte $06, $30, $31, $32, $33, $34, $35
clock4:         .byte 25, "YYYY-MM-DDTHH:MM:SS+HHMM", 0
clock5:         .byte 25, "2025-06-11T20:19:00+0100", 0
