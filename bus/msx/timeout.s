	public	timeout_init, timeout_check, timeout_cleanup

; ============================================================
; Timeout routines using MSX BIOS JIFFY counter
; ------------------------------------------------------------
; timeout_init  - HL = timeout duration (in jiffies)
;                 pushes timeout and current JIFFY on stack
; timeout_check - sets Carry if timeout elapsed
; timeout_cleanup - pops saved variables from stack
; ============================================================

	JIFFY    EQU	0xFC9E

; ------------------------------------------------------------
; timeout_init
;   Input: HL = timeout duration (16-bit)
;   Stack: pushes 2-byte target time
;   Destroys: HL, BC
; ------------------------------------------------------------
timeout_init:
	pop	bc		; save return address
	push	hl		; push TIMEOUT
	ld	hl,(JIFFY)	; get current JIFFY value
	push	hl		; push START on stack
	push	bc		; restore return address
	ret

; ------------------------------------------------------------
; timeout_check
;   Checks if timeout has expired by calculating delta of CUR - START
;   Sets Carry if timeout elapsed.
;   Destroys: A, BC, DE, HL
; ------------------------------------------------------------
timeout_check:
	pop	hl		; save return address
	pop 	de		; DE = START
	pop 	bc		; BC = TIMEOUT
	push	bc		; put TIMEOUT back on stack
	push	de		; put START back on stack
	push	hl		; restore return address

	ld	hl,(JIFFY)	; HL = CUR

	xor	a		; clear carry before subtract
	sbc	hl,de		; HL = CUR - START
	xor	a		; clear carry before subtract
	sbc	hl,bc		; sets carry if (CUR - START) < TIMEOUT (borrow occurred)
	ccf			; invert it: carry = 1 if (CUR - START) >= TIMEOUT
	ret

; ------------------------------------------------------------
; timeout_cleanup
;   Discards START and TIMEOUT from top of stack
;   Destroys: AF
; ------------------------------------------------------------
timeout_cleanup:
	pop	af		; save return address
	inc	sp		; discard a byte
	inc	sp		; discard a byte
	inc	sp		; discard a byte
	inc	sp		; discard a byte
	push	af		; restore return address
	ret
