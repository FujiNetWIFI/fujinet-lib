	include	"portio.inc"

;; extern int __FASTCALL__ port_getc_timeout(uint16_t timeout);
;; timeout is in HL
;; loop until data is available or timeout elapses
;; Destroys: AF, BC, HL
;; Preserves: DE, IX, IY
_port_getc_timeout:
    ; HL = timeout in milliseconds

@millisecond:
    ; Check for a character while waiting approximately 1 ms.
    ; At 4 MHz, this loop is approximately 20 us per iteration.
	ld	b,50

@poll:
	xor	a
	out	(SIO_CTRL),a
	in	a,(SIO_CTRL)
	bit	SIO_RX_READY,a
	jr	nz,@got_character

	djnz	@poll

    ; One millisecond has elapsed.
	dec	hl
	ld	a,h
	or	l
	jr	nz,@millisecond

@timeout:
	ld	hl,-1
	ret

@got_character:
	in	a,(SIO_DATA)
	ld	h,0
	ld	l,a
	ret
