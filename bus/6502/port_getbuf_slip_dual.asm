	.importzp   sp, ptr1, ptr2, ptr3, ptr4
	.import     popax
	.export     _port_getbuf_slip_dual
	.import     _port_getc_timeout

	; --- Constants
	SLIP_END      = $C0
	SLIP_ESC      = $DB
	SLIP_ESC_END  = $DC
	SLIP_ESC_ESC  = $DD

; --- Register Mappings for Clarity ---
; ptr1 = Current Buffer Pointer
; ptr2 = Data Buffer Pointer
; ptr3 = Total Written Counter
; ptr4 = Header Length (temp)

.segment "CODE"

_port_getbuf_slip_dual:
	; On entry:
	; A/X = timeout (rightmost param)
	; Software Stack (sp) contains (from top to bottom):
	;   data_len (2 bytes)
	;   data_buf (2 bytes)
	;   hdr_len  (2 bytes)
	;   hdr_buf  (2 bytes)

	; 1. Save timeout for later use
	sta timeout_val
	stx timeout_val+1

	; 2. Pull arguments from cc65 software stack
	jsr popax       ; Get data_len
	sta len_data
	stx len_data+1

	jsr popax       ; Get data_buf
	sta ptr2
	stx ptr2+1

	jsr popax       ; Get hdr_len
	sta len_curr    ; We'll use this as our active "remaining" counter
	stx len_curr+1

	jsr popax       ; Get hdr_buf
	sta ptr1        ; We'll use this as our active pointer
	stx ptr1+1

	; 3. Initialize total written counter
	lda #0
	sta ptr3
	sta ptr3+1

; --- Phase 1: Sync ---
slipd_sync:
	jsr call_getc
	bcs slipd_done      ; If carry is set then timed out
	cmp #SLIP_END
	bne slipd_sync

; --- Phase 2: Skip consecutive ENDs ---
slipd_skip_end:
	jsr call_getc
	bcs slipd_done      ; If carry is set then timed out
	cmp #SLIP_END
	beq slipd_skip_end

; --- Phase 3: Decode Loop ---
slipd_decode_loop:
	cmp #SLIP_END
	beq slipd_done

	cmp #SLIP_ESC
	beq slipd_handle_escape

slipd_store_byte:
	ldy #0
	sta (ptr1),y        ; Store to current buffer

	; inc ptr1 (16-bit)
	inc ptr1
	bne :+
	inc ptr1+1
:   ; inc ptr3 (Total Written)
	inc ptr3
	bne :+
	inc ptr3+1
:
	; dec len_curr (16-bit)
	lda len_curr
	bne :+
	dec len_curr+1
:   dec len_curr

	; Check if current buffer full
	lda len_curr
	ora len_curr+1
	bne slipd_next_char

	; Switch to data buffer if available
	lda len_data
	ora len_data+1
	beq slipd_done      ; No more buffer space

	; Load data buffer into active ptr1/len_curr
	lda ptr2:   sta ptr1
	lda ptr2+1: sta ptr1+1
	lda len_data:   sta len_curr
	lda len_data+1: sta len_curr+1

	; Mark data buffer as "used"
	lda #0
	sta len_data
	sta len_data+1

slipd_next_char:
	jsr call_getc
	bcc slipd_decode_loop
	; else timeout, fall through

slipd_done:
	lda ptr3            ; Return total written in A/X
	ldx ptr3+1
	rts

slipd_handle_escape:
	jsr call_getc
	bcs slipd_done      ; If carry is set then timed out
	cmp #SLIP_ESC_END
	beq @is_end
	cmp #SLIP_ESC_ESC
	beq @is_esc
	jmp slipd_store_byte ; store unknown as-is
@is_end:
	lda #SLIP_END
	jmp slipd_store_byte
@is_esc:
	lda #SLIP_ESC
	jmp slipd_store_byte

; Internal wrapper to handle cc65 fastcall to getc
call_getc:
	lda timeout_val
	ldx timeout_val+1
	jsr _port_getc_timeout
	; We assume _port_getc_timeout returns char in A and sets Carry on timeout
	rts

.segment "BSS"
timeout_val: .res 2
len_data:    .res 2
len_curr:    .res 2
