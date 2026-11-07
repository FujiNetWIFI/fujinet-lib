; crt0.s -- startup for a FujiNet Atari 5200 client, in place of cc65's.
;
; The header's $BFFD is $FF, so the BIOS jumps straight here with nothing
; set up. This does what the BIOS would have, without depending on which
; BIOS it is: the chips cleared, CHBASE at the BIOS font, the RAM vectors at
; $0200-$021B, a VBI and keypad handler that keep the BIOS's zero-page
; layout, and display DMA on as the BIOS leaves it -- over a blank display
; list here, never an unset one ANTIC could aim at the mailbox -- so cc65's
; atari5200 library works unchanged on top of them. The keypad's IRQ is on;
; fuji_a5200_key holds the last key.
;
; The console's NMI and IRQ vectors are the BIOS's, and both revisions enter
; through $FCA2 / $FC00 to JMP (VVBLKI) / JMP (VIMIRQ) with nothing pushed:
; these handlers save what they use.

        .export         __STARTUP__ : absolute = 1
        .export         _exit, start, _fuji_a5200_key
        .import         __STACK_START__, __STACK_SIZE__
        .import         initlib, donelib, zerobss, copydata, callmain

        .include        "zeropage.inc"

POKMSK  = $00
RTCLOKH = $01
RTCLOKL = $02
CRITIC  = $03
ATRACT  = $04
SDLSTL  = $05
SDLSTH  = $06
SDMCTL  = $07
PCOLR0  = $08
PADDL0  = $11
VIMIRQ  = $0200
VVBLKD  = $0204
VKYBDI  = $0208
VKYBDF  = $020A
GTIA    = $C000
COLPM0  = $C012
ANTIC   = $D400
DMACTL  = $D400
DLISTL  = $D402
DLISTH  = $D403
CHBASE  = $D409
NMIEN   = $D40E
POKEY   = $E800
POT0    = $E800
KBCODE  = $E809
POTGO   = $E80B
IRQEN   = $E80E
IRQST   = $E80E
SKCTL   = $E80F
HOLD_FRAMES = 4

        .segment "STARTUP"
start:  sei
        cld
        ldx     #$FF
        txs
        lda     #0
        sta     NMIEN
        sta     DMACTL
        sta     IRQEN
        tax
@clr:   sta     POKEY,x         ; every chip register to 0, as the BIOS does
        sta     GTIA,x
        sta     ANTIC,x
        sta     $00,x
        sta     $0200,x
        inx
        bne     @clr
        ldx     #vecs_end - vecs - 1
@vec:   lda     vecs,x
        sta     VIMIRQ,x
        dex
        bpl     @vec
        lda     #$F8            ; the BIOS font, which cc65's conio expects
        sta     CHBASE
        lda     #<blank
        sta     SDLSTL
        lda     #>blank
        sta     SDLSTH
        lda     #$22            ; normal playfield, display-list DMA
        sta     SDMCTL
        lda     #<(__STACK_START__ + __STACK_SIZE__)
        sta     c_sp
        lda     #>(__STACK_START__ + __STACK_SIZE__)
        sta     c_sp+1
        jsr     zerobss
        jsr     copydata
        lda     #$02            ; keypad scan on
        sta     SKCTL
        lda     #$40            ; its IRQ, alone
        sta     POKMSK
        sta     IRQEN
        lda     #$C0            ; VBI and DLIs, as the BIOS leaves NMIEN
        sta     NMIEN
        cli
        jsr     initlib
        jsr     callmain
_exit:  jsr     donelib
@hang:  jmp     @hang

        .rodata

vecs:   .word   irq, vbi, vbi_exit, dli, kbd, kbd_done
        .word   pla_rti, pla_rti, pla_rti, pla_rti, pla_rti, pla_rti, pla_rti, pla_rti
vecs_end:

keys:   .byte   $FF, $0B, $00, $0A, $0E, $09, $08, $07
        .byte   $0D, $06, $05, $04, $0C, $03, $02, $01

        .code

; The BIOS's $FCB8, register for register.
.proc vbi
        pha
        txa
        pha
        tya
        pha
        inc     RTCLOKL
        bne     :+
        inc     ATRACT
        inc     RTCLOKH
:       lda     hold            ; a held key's repeats keep this above zero
        beq     :+
        dec     hold
:       lda     CRITIC
        bne     vbi_exit
        lda     SDLSTH
        sta     DLISTH
        lda     SDLSTL
        sta     DLISTL
        lda     SDMCTL
        sta     DMACTL
        ldy     ATRACT
        bpl     :+
        ldy     #$80
        sty     ATRACT
:       ldx     #8
@col:   lda     PCOLR0,x
        cpy     #$80
        bcc     :+
        eor     RTCLOKH
        and     #$F6
:       sta     COLPM0,x
        dex
        bpl     @col
        ldx     #7
@pot:   lda     POT0,x
        sta     PADDL0,x
        dex
        bpl     @pot
        sta     POTGO
        jmp     (VVBLKD)
.endproc

.proc vbi_exit
        pla
        tay
        pla
        tax
        pla
        rti
.endproc

dli:    rti

; Only the keypad's IRQ is enabled here; anything else is acknowledged.
.proc irq
        pha
        lda     IRQST
        and     #$40
        bne     @other
        lda     #$BF
        sta     IRQEN
        lda     POKMSK
        sta     IRQEN
        jmp     (VKYBDI)
@other: lda     #0
        sta     IRQEN
        lda     POKMSK
        sta     IRQEN
        pla
        rti
.endproc

pla_rti:
        pla
        rti

; The BIOS's $FD02: KBCODE through its table, then VKYBDF.
.proc kbd
        txa
        pha
        tya
        pha
        lda     KBCODE
        lsr     a
        and     #$0F
        tax
        lda     keys,x
        jmp     (VKYBDF)
.endproc

; 0-9, $0A '*', $0B '#', $0C START, $0D PAUSE, $0E RESET; stored + 1.
; Without debounce (SKCTL $02, the BIOS's) POKEY interrupts on every scan of
; a held key, and SKSTAT's key-down bit drops between scans: a repeat of the
; key while `hold` runs only restarts it, so each press counts once.
.proc kbd_done
        ldx     #HOLD_FRAMES
        cmp     held
        bne     @new
        ldy     hold
        stx     hold
        cpy     #0
        bne     @same
@new:   stx     hold
        sta     held
        clc
        adc     #1
        sta     _fuji_a5200_key
        lda     #0
        sta     ATRACT
@same:  jmp     vbi_exit
.endproc

        .segment "DLIST"
blank:  .byte   $70, $70, $70, $41, <blank, >blank

        .bss
_fuji_a5200_key:
        .res    1
held:   .res    1               ; the last key
hold:   .res    1               ; frames until it may count again

        .segment "CLAIM"
        .byte   "FUJI", 1, 0, 0             ; version 1, kind from the size
        .byte   $02                         ; $BFE7: PAL-compatible, for the 2-port BIOS

        .segment "CARTHDR"
        .byte   "FUJINET             "      ; $BFE8: 20 characters, unseen
        .byte   $FF, $FF                    ; $BFFC: year; $BFFD = $FF: no BIOS title
        .word   start                       ; $BFFE: entry
