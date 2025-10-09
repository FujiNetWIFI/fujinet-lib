	public	_port_getc, _port_getc_timeout, _port_getbuf
	public	_port_putc, _port_putbuf

	extern	timeout_init, timeout_check, timeout_cleanup

	IO_OFFSET	EQU	(0x4000 + 0x3FFC)
	IO_GETC		EQU	(IO_OFFSET + 0)
	IO_STATUS	EQU	(IO_OFFSET + 1)
	IO_PUTC		EQU	(IO_OFFSET + 2)
	IO_CONTROL	EQU	(IO_OFFSET + 3)

;; extern int __FASTCALL__ port_getc();
;; returns signed int with data or -1 if no data is available
_port_getc:
	ld	a,(IO_STATUS)
	bit	7,a		; set Z flag based on high bit
	jr	nz,get_data	; if high bit is set there is data
	ld	hl,0xFFFF	; no data avail, return -1
	ret

get_data:
	ld	a,(IO_GETC)
	ld	l,a		; transfer data to L register
	ld	h,0		; set high byte to 0
	ret

;; extern int __FASTCALL__ port_getc_timeout(uint16_t timeout);
;; timeout is in HL
;; loop until data is available or timeout elapses
_port_getc_timeout:
	push	bc		; save BC and DE because
	push	de		; timeout_check destroys them
	call	timeout_init
wait_rx:
	call	_port_getc	; get byte if there is one
	jr	nz,getc_done	; Z80 LD doesn't change flags so NZ check still works
	call	timeout_check
	jr	nc,wait_rx	; if carry is clear then haven't timed out

	ld	hl,0xFFFF	; timed out, return -1
getc_done:
	call	timeout_cleanup
	pop	de		; restore saved DE
	pop	bc		; restore saved BC
	ret
	
;; extern uint16_t __CALLEE__ port_getbuf(void *buf, uint16_t len, uint16_t timeout);
;; returns length of data received in HL
_port_getbuf:
	pop	ix		; save return address
	pop	de		; timeout
	pop	bc		; len
	pop	iy		; buffer address
	push	ix		; restore return address

	ld	ix,0		; zero out received length

next_char:	
	ld 	a,b		; check if length remaining
	or 	c		; merge low byte
	jr	z,getbuf_done

	ld	hl,de		; set timeout
	call	_port_getc_timeout
	ld	a,h		; check high byte
	or	a
	jr	nz,getbuf_done	; if high byte is set then timed out
	ld	a,l		; received byte is in L
	ld	(iy),a		; save byte to pointer
	inc	iy		; increment to next address
	dec	bc		; decrement length
	inc	ix		; increment length received
	jr	next_char	; check if there's more to receive

getbuf_done:
	ld	hl,ix		; return length received
	ret
	
;; extern int __FASTCALL__ port_putc(uint8_t c);
;; writes data in L to port, no return value
_port_putc:
	ld	a,l		; can't write L directly to address
	ld	(IO_PUTC),a	; send data out port
	ret

;; extern uint16_t __CALLEE__ port_putbuf(void *buf, uint16_t len);
;; returns number of bytes sent in HL
_port_putbuf:
	pop	ix		; save return address
	pop	bc		; len
	pop	iy		; buffer address
	push	ix		; restore return address
	ld	ix,bc		; cheat: will always return length
    
putc:
	ld 	a,b		; check if length remaining
	or 	c		; merge low byte
	jr	z,putbuf_done

	ld	a,(iy)		; load byte to send
	ld	(IO_PUTC),a	; send data out port
	inc 	iy		; increment to next address
	dec 	bc		; decrement length
	jr	putc		; check if more to transmit

putbuf_done:	
	ld	hl,ix		; return length transmitted in HL
	ret
