; fuji_atari5200_go.s -- the parts that must be exact 6502: the mailbox's
; hotspot reads, and leaving for the next image through the cart's stub.
;
; The edge has no R/W, so register n = v is a read of $B500+n then $B600+v,
; and a TX byte b a read of $B700+b. Each is one `lda base,y` from a
; page-aligned base: never (zp),Y, never an RMW, never a store.

        .export _fn_tx, _fn_regwr, _fuji_a5200_boot, _fuji_a5200_config
        .import popa
        .importzp tmp1

REGSEL  = $B500
REGDATA = $B600
TXDATA  = $B700
STUB    = $B5F0
ARMED   = $B41C
REG_BOOTLOCK = $11
REG_CONFIG = $14
BOOTLOCK_MAGIC = $B5
CONFIG_MAGIC = $C5

RTCLOKL = $02
GTIA    = $C000
GRACTL  = $C01D
ANTIC   = $D400
DMACTL  = $D400
VCOUNT  = $D40B
NMIEN   = $D40E
NMIRES  = $D40F
POKEY   = $E800
IRQEN   = $E80E

        .code

; void __fastcall__ fn_tx (uint8_t b)
.proc _fn_tx
        tay
        lda     TXDATA,y
        rts
.endproc

; void __fastcall__ fn_regwr (uint8_t reg, uint8_t val)
.proc _fn_regwr
        sta     tmp1
        jsr     popa
        tax
        lda     REGSEL,x
        ldy     tmp1
        lda     REGDATA,y
        rts
.endproc

; bool fuji_a5200_boot (void) -- arm the staged image and hand the console
; to it. Returns (false) only if the cart never armed the swap.
.proc _fuji_a5200_boot
        lda     REGSEL+REG_BOOTLOCK
        ldy     #BOOTLOCK_MAGIC
        lda     REGDATA,y
        jmp     handover
.endproc

; bool fuji_a5200_config (void) -- back to CONFIG; as fuji_a5200_boot.
.proc _fuji_a5200_config
        lda     REGSEL+REG_CONFIG
        ldy     #CONFIG_MAGIC
        lda     REGDATA,y
        jmp     handover
.endproc

; Wait up to ~2 s for ARMED; then quiet the machine as a power-on leaves it
; and jump to the cart's stub, whose JMP ($FFFC) is the last cart read
; before the BIOS's reset code; the read of its last byte swaps.
.proc handover
        ldx     #120
        lda     RTCLOKL
        sta     tmp1
wait:   lda     ARMED
        bne     armed
        lda     RTCLOKL
        cmp     tmp1
        beq     wait
        sta     tmp1
        dex
        bne     wait
        lda     #0              ; false
        tax
        rts
armed:  sei
        lda     #0
        sta     NMIEN           ; first: a pending NMI would run through $0202
        sta     NMIRES
        sta     DMACTL          ; no display-list fetch may reach the new image
        sta     GRACTL
        sta     IRQEN
vbl:    lda     VCOUNT          ; let the frame's DMA finish
        cmp     #124
        bcc     vbl
        lda     #0
        tax
chips:  sta     POKEY,x
        sta     GTIA,x
        sta     ANTIC,x
        inx
        bne     chips
ramlo:  .repeat 32, I           ; two halves: one is past a branch's reach
        sta     I * $100,x
        .endrepeat
        inx
        bne     ramlo
ramhi:  .repeat 32, I
        sta     $2000 + I * $100,x
        .endrepeat
        inx
        bne     ramhi
        clc                     ; the flags as the BIOS's own start leaves them
        jmp     STUB
.endproc
