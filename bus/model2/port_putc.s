	include	"portio.inc"
	public	_port_putc

;; extern int __FASTCALL__ port_putc(uint8_t c);
;; writes data in L to port, no return value
_port_putc:
	in	a,(SIO_CTRL)	; get transmit flags
	bit	SIO_TX_EMPTY,a	; check if able to send
	jr	z,_port_putc	; not yet
	ld	a,l		; can't write L directly to address
	out	(SIO_DATA),a	; send data out port
	ret
