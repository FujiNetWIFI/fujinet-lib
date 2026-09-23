	extern	_port_getc_timeout

	extern	getbuf_done

	extern	timeout_init, timeout_check, timeout_cleanup

	SLIP_END	EQU	0xC0
	SLIP_ESC	EQU	0xDB
	SLIP_ESC_END	EQU	0xDC
	SLIP_ESC_ESC	EQU	0xDD

	SIO_DATA	EQU	0xF4
	SIO_CTRL	EQU	0xF6
	CTC_RX		EQU	0xF0
	CTC_TX		EQU	0xF1
	SIO_RX_READY	EQU	0
	SIO_TX_EMPTY	EQU	2
SIO_RESET EQU $18
; WR1:
;   interrupts disabled
SIO_WR1 EQU $00

; WR3:
;   auto enables
;   8-bit receive
;   receiver enabled
SIO_WR3 EQU $C1

; WR5:
;   8-bit transmit
;   transmitter enabled
;   RTS/DTR
SIO_WR5 EQU $EA

; ----------------------------------------------------------------------
; CTC
;
; 0x47:
;   interrupts disabled
;   counter mode
;   rising edge
;   time constant follows
; ----------------------------------------------------------------------

CTC_COUNTER EQU $47
