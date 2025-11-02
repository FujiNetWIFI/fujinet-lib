	public	_port_getc, _port_getc_timeout, _port_discard_until
	public	_port_getbuf, _port_get_until
	public	_port_putc, _port_putbuf

	extern	timeout_init, timeout_check, timeout_cleanup

	IO_OFFSET	EQU	(0x8000 + 0x3FFC)
	IO_GETC		EQU	(IO_OFFSET + 0)
	IO_STATUS	EQU	(IO_OFFSET + 1)
	IO_PUTC		EQU	(IO_OFFSET + 2)
	IO_CONTROL	EQU	(IO_OFFSET + 3)

;; extern int __FASTCALL__ port_getc();
;; returns signed int with data or -1 if no data is available, zero flag clear if data
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
	jr	nz,getc_done	; zero flag clear if data
	call	timeout_check
	jr	nc,wait_rx	; if carry is clear then haven't timed out

	ld	hl,0xFFFF	; timed out, return -1
getc_done:
	call	timeout_cleanup
	pop	de		; restore saved DE
	pop	bc		; restore saved BC
	ret

;; extern int __CALLEE__ port_discard_until(uint8_t c, uint16_t timeout);
;; reads data until c is seen or timeout occurs, returns c or -1 on timeout
;; timeout *does not reset* when a character is received
_port_discard_until:
	pop	ix		; save return address
	pop	hl		; timeout
	pop	de		; char to look for
	push	ix		; restore return address

	call	timeout_init

not_yet:
	call	_port_getc	; get byte if there is one
	jr	nz,check_c	; zero flag clear if data
	push	de
	call	timeout_check
	pop	de
	jr	nc,not_yet	; if carry is clear then haven't timed out

	ld	hl,0xFFFF	; timed out, return -1
	jr	discard_done

check_c:
	ld	a,l		; check low byte
	cp	e
	jr	nz,not_yet	; try again

discard_done:
	call	timeout_cleanup
	ret

;; extern uint16_t __CALLEE__ port_getbuf(void *buf, uint16_t len, uint16_t timeout);
;; returns length of data received in HL
;; timeout resets when a character is received
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

;; extern uint16_t __CALLEE__ port_get_until(void *buf, uint16_t maxlen, uint8_t c,
;;					     uint16_t timeout);
;; reads until c is found, timeout, or maxlen is reached
;; returns length of data received, including c
;; timeout resets when a character is received
_port_get_until:
	pop	ix		; save return address
	pop	de		; timeout
	pop	bc		; char to look for
	ld	a,c		; save char in A register
	pop	bc		; maxlen
	pop	iy		; buffer address
	push	ix		; restore return address

	push	af		; save sentinel char we're searching for
	ld	ix,0		; zero out received length

next_until:
	ld 	a,b		; check if length remaining
	or 	c		; merge low byte
	jr	z,getuntil_done

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

	pop	hl		; restore sentinel char we're waiting for
	push	hl		; keep it for next time
	cp	h		; is this the end?
	jr	nz,next_until	; more to receive

getuntil_done:
	pop	hl		; discard sentinel char off stack
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
