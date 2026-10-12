; Port routines for the FujiNet S-100 card, 8080 only.
;
; The card's pipe is four ports at a DIP-switched base: +0 GETC, +1 STATUS,
; +2 PUTC, +3 CONTROL (write) or TICK (read, milliseconds). The 8080 has no
; IN r,(C), so port_init finds the base and patches it into every IN/OUT here.
; sccz80: arguments on the stack, result in HL.

        SECTION code_user

        PUBLIC  _port_init
        PUBLIC  _port_getc_timeout
        PUBLIC  _port_putc

FNBASE  equ     0A0h            ; the card's default DIP setting
ST_AVAIL equ    80h
ST_SIG  equ     2Ch             ; STATUS bits 6-1 on a card
ST_SIGM equ     7Eh
ST_TXF  equ     01h
CTL_FLUSH equ   08h

; Z set if the STATUS register at base A + 1 looks like the card.
probe:  inc     a
        ld      (prb+1),a
prb:    in      a,(0FFh)
        and     ST_SIGM
        cp      ST_SIG
        ret

; void port_init(void): the default base, else a scan that skips the FDC
; (08h) and a 2SIO console (10h). No card: keep the default and time out.
_port_init:
        ld      a,FNBASE
        call    probe
        ld      a,FNBASE
        jp      z,set
        ld      c,0
scan:   ld      a,c
        cp      08h
        jp      z,next
        cp      10h
        jp      z,next
        call    probe
        ld      a,c
        jp      z,set
next:   ld      a,c
        add     a,4
        ld      c,a
        jp      nz,scan
        ld      a,FNBASE
set:    ld      (getc+1),a
        inc     a
        ld      (stat1+1),a
        ld      (stat2+1),a
        inc     a
        ld      (putc+1),a
        inc     a
        ld      (ctl+1),a
        ld      (tick1+1),a
        ld      (tick2+1),a
        ld      a,CTL_FLUSH
ctl:    out     (0FFh),a
        ret

; int port_getc_timeout(uint16_t timeout): timeout in TICK milliseconds
_port_getc_timeout:
        pop     bc
        pop     de
        push    de
        push    bc
tick1:  in      a,(0FFh)
        ld      c,a             ; last tick seen
wait:
stat1:  in      a,(0FFh)
        and     ST_AVAIL
        jp      nz,got
tick2:  in      a,(0FFh)
        ld      b,a
        sub     c               ; ms since the last look
        jp      z,wait
        ld      c,b
        ld      l,a
        ld      a,e
        sub     l
        ld      e,a
        ld      a,d
        sbc     a,0
        ld      d,a
        jp      c,expired
        or      e
        jp      nz,wait
expired:
        ld      hl,-1
        ret
got:
getc:   in      a,(0FFh)
        ld      l,a
        ld      h,0
        ret

; void port_putc(uint8_t c): TXFULL is advisory, so wait only a little
_port_putc:
        pop     bc
        pop     hl
        push    hl
        push    bc
        ld      b,0
room:
stat2:  in      a,(0FFh)
        and     ST_TXF
        jp      z,send
        dec     b
        jp      nz,room
send:   ld      a,l
putc:   out     (0FFh),a
        ret
