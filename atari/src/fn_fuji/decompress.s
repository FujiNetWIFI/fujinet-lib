; decompress.s
;
; converted from original at http://www.retrosoftware.co.uk/forum/viewtopic.php?f=73&t=999

        .export   _decompress

        .import   popax
        .include  "macros.inc"
        .include  "zp.inc"

; void decompress(void *src, void *dst)
.proc   _decompress
        ; move args into ptr1/2
        axinto  ptr2            ; dst
        popax   ptr1            ; src

        ldx     #$00
for:
        lda     (ptr1, x)       ; next control byte
        beq     done            ; 0 signals end of decompression
        bpl     copy_raw        ; msb=0 means just copy this many bytes from source
        clc
        adc     #$82            ; flip msb, then add 2, we wont request 0 or 1 as that wouldn't save anything
        sta     tmp1            ; count of bytes to copy (>= 2)
        ldy     #$01            ; byte after control is offset
        lda     (ptr1), y       ; offset from current t1 - 256
        tay

        lda     #$02            ; advance t1 past the control byte and offset
        clc
        adc     ptr1
        sta     ptr1
        bcc     copy_previous
        inc     ptr1+1

copy_previous:
        dec     ptr2+1
        lda     (ptr2), y
        inc     ptr2+1
        sta     (ptr2,x)
        inc     ptr2
        bne     :+
        inc     ptr2+1

:       dec     tmp1
        bne     copy_previous
        beq     for

copy_raw:
        tay

copy:
        inc     ptr1
        bne     :+
        inc     ptr1+1
:       dey
        bmi     for
        mva     {(ptr1,x)}, {(ptr2,x)}
        inc     ptr2
        bne     :+
        inc     ptr2+1
:       bne     copy            ; always unless high byte rolled over from FF to 00, which would mean wrapping around memory

done:
        rts

.endproc