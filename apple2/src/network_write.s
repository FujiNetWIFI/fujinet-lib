        .export     _network_write

        .import     _bad_unit
        .import     _fn_device_error
        .import     _fn_error
        .import     _memcpy
        .import     _sp_clr_pay
        .import     _sp_network
        .import     _sp_payload
        .import     _sp_write
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

.proc _network_write
        axinto  tmp7            ; len into tmp7/8

        jsr     _sp_clr_pay     ; calls bzero, so trashes p1/2/3
        ldy     #$00
        sty     _fn_device_error

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

        ; remove the function args we didn't read from the stack, save the real error, and return bad command
        jsr     incsp4
        jmp     _bad_unit

        ; check the buffer is not null
:       popax   tmp9            ; buffer location into tmp9/10
        ora     tmp10           ; it's 0 if both bytes are 0
        bne     skip_devicespec

        ; bad buffer, remove 2 bytes from stack and return bad command
        jsr     incsp2
        lda     #SP_ERR_BAD_CMD
        jmp     _fn_error

skip_devicespec:
        ; remove parameter from call stack
        jsr     incsp2

while_len:
        ; push network id into stack for call to sp_write
        pusha   _sp_network

        ; use the minimum of MAX_READ_SIZE (512) or len
        lda     tmp8            ; hi byte of len (tmp7/8)
        cmp     #$2             ; 512 high byte
        bcc     @len_under_512
        ; len >= 512, so cap at max value of 512
        lda     #$00
        sta     tmp5
        ldx     #$02
        stx     tmp6
        bne     :+              ; always

@len_under_512:
        lda     tmp7
        ldx     tmp8
        sta     tmp5
        stx     tmp6

        ; copy tmp5/6 bytes from buffer into sp_payload
:       pushax  #_sp_payload    ; dest
        pushax  tmp9            ; src
        setax   tmp5            ; len
        jsr     _memcpy

        setax   tmp5            ; len
        jsr     _sp_write
        bne     write_err

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

write_err:
        ; convert device error to library error and return
        jmp     _fn_error

.endproc
