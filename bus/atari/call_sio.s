	.export _call_sio

.proc _call_sio
	jsr	$E459
	bpl	success
	lda	#0
	rts
	success:
	lda	#1
	rts
.endproc
