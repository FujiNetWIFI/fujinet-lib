        .export     _network_read

        .import     _bad_unit
        .import     _fn_bytes_read
        .import     _fn_device_error
        .import     _fn_error
        .import     _fn_network_bw
        .import     _memcpy
        .import     _sp_clr_pay
        .import     _sp_network
        .import     _sp_payload
        .import     _sp_read
        .import     _sp_status
        .import     incsp2
        .import     incsp4
        .import     popax
        .import     pusha
        .import     pushax
        .import     return0

        .include    "fujinet-network.inc"
        .include    "macros.inc"
        .include    "sp.inc"
        .include    "zp.inc"

; network_read(char* devicespec, uint8_t *buf, uint16_t len);
;
.proc _network_read
        axinto  tmp7            ; len into tmp7/8

        ; ---------------------------------------------------------------------------------------------
        ; ARGS PARSING AND VALIDATION
        ; ---------------------------------------------------------------------------------------------

        ; clear memory
        jsr     _sp_clr_pay
        ldy     #$00
        sty     _fn_device_error
        sty     _fn_bytes_read
        sty     _fn_bytes_read+1

        lda     tmp7
        ora     tmp8            ; check len > 0
        bne     :+

        ; remove the function args we didn't read from the stack, save the real error, and return bad command
        jsr     incsp4
        lda     #SP_ERR_BAD_CMD
        jmp     _fn_error

        ; check network id is set
:       lda     _sp_network     ; get network id
        bne     :+

        ; didn't find the network unit.
        ; remove the function args we didn't read from the stack, save the real error, and return bad unit
        jsr     incsp4
        jmp     _bad_unit

        ; check the buffer is not null
:       popax   tmp9            ; buffer location into tmp9/10
        ora     tmp10           ; it's 0 if both bytes are 0
        bne     :+

        ; bad buffer, remove 2 bytes from stack and return bad command
        jsr     incsp2
        lda     #SP_ERR_BAD_CMD
        jmp     _fn_error

        ; remove the devicespec, still on the call args stack
        jsr     incsp2

        ; ---------------------------------------------------------------------------------------------
        ; GET BYTE COUNT (NETWORK STATUS)
        ; ---------------------------------------------------------------------------------------------

:       pusha   _sp_network
        lda     #'S'
        jsr     _sp_status
        bne     read_err

        mwa     _sp_payload, _fn_network_bw

        ; check the bytes waiting count, if it's under len, use bw instead
        lda     tmp8                    ; hi byte of count
        cmp     _fn_network_bw+1
        bne     :+
        lda     tmp7
        cmp     _fn_network_bw
:       bcc     while_len               ; no need to alter len, it's under BW

        ; len >= bw, so use bw instead as we can only get a max of bw bytes.
        mwa     _fn_network_bw, tmp7

; ---------------------------------------------------------------------------------------------
; READ LOOP IN 512 BYTE BLOCKS (or Bytes Waiting if lower)
; ---------------------------------------------------------------------------------------------

; len (in tmp7/8) is the outstanding bytes to read overall
; tmp5/6 holds count of the number of bytes we will fetch in this round
; _fn_bytes_read is the cumulative count of bytes read
while_len:
        ; use the minimum of MAX_READ_SIZE (512) or len, which decreases as we read blocks
        lda     tmp8            ; hi byte of len (tmp7/8)
        cmp     #$2             ; 512 high byte
        bcc     len_under_512
        ; len >= 512, so cap at max value of 512
        lda     #$00
        sta     tmp5
        ldx     #$02
        stx     tmp6
        bne     :+              ; always

read_err:
        ; convert device error to library error and return
        jmp     _fn_error

len_under_512:
        setax   tmp7            ; last block of bytes to load
        axinto  tmp5            ; copy it to amount we will request

        ; ---------------------------------------------------------------------------------------------
        ; PERFORM READ
        ; ---------------------------------------------------------------------------------------------

        ; add the count of requested bytes to total
:       adw     _fn_bytes_read, tmp5

        pusha   _sp_network
        setax   tmp5            ; restore count for sp_read
        jsr     _sp_read
        bne     read_err

        ; copy tmp5/6 bytes into buffer from sp_payload
        pushax  tmp9            ; dst tmp9/10 (memcpy trashes ptr1-3)
        pushax  #_sp_payload    ; src
        setax   tmp5            ; request length (either 512, or lower if on last block)
        jsr     _memcpy

        ; move the buffer pointer on by len
        adw     tmp9, tmp5      ; tmp9/10 increased by tmp5/6

        ; decrease len by amount transferred
        sbw     tmp7, tmp5      ; tmp7/8 decreased by tmp5/6

        ; have we finished?
        lda     tmp7
        ora     tmp8
        bne     while_len

        ; no more data, no errors reported, return OK
        jmp     return0         ; FN_ERR_OK

.endproc