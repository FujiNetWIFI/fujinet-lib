		.export 	_fuji_cbm_open

		.import 	_cbm_k_open
		.import 	_cbm_k_setlfs
		.import 	popa
		.import     ___oserror

        .include    "cbm_kernal.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; A replacement for cbm_open that takes parameters for location of the data to send to SETNAM and the size
; instead of computing it, which allows us to send binary data as part of the NAME field to pass parameters to FN.

; uint8_t __fastcall__ fuji_cbm_open(
;    uint8_t  lfn,
;    uint8_t  device,
;    uint8_t  sec_addr,
;    uint8_t  len,
;    uint8_t* name
; );

_fuji_cbm_open:
		stx 	tmp1			; save high byte of name address
		tax						; X = ADDR LOW
		jsr 	popa			; A = LEN (leaves X alone)
        ldy     tmp1			; Y = ADDR HIGH
        jsr     SETNAM

		jsr 	popa			; A = SA
		jsr 	_cbm_k_setlfs	; Call SETLFS, pops DEV and LFN

		jsr     _cbm_k_open		; Call OPEN
		sta 	___oserror
		rts
