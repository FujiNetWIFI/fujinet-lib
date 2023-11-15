        .export     _network_ioctl

        .import     _fn_device_error
        .import     _fn_error
        .import     _memcpy
        .import     _sp_clr_payload
        .import     _sp_control
        .import     _sp_init
        .import     _sp_is_init
        .import     _sp_network
        .import     _sp_payload
        .import     addysp
        .import     incsp2
        .import     popa
        .import     popax
        .import     pusha
        .import     pushax

        .include    "fujinet-network.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, ...);
;
;
; uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, int16_t use_aux, void *buffer, uint16_t len);
;
; if use_aux is true (1), we put aux1/2 into payload[2,3], and any buffer will copy len bytes - 2 after it to payload[4+].
; if use_aux is false, copy buffer to payload[2+]
; if buffer is NULL, no copying every done from buffer
; len is set at payload[0,1]

.proc _network_ioctl
        cpy     #11                     ; 5 for standard args, 3 extra args at 2 bytes each = 5 + 6 = 11 bytes on stack. if not, we have been called incorrectly
        beq     :+

args_error:
        ; increase SP by Y to clear the params we received, and return error
        jsr     addysp
        ldx     #$00
        stx     _fn_device_error        ; no device error, it's just bad args
        lda     #FN_ERR_BAD_CMD
        rts

        ; this is one function that may be called outside an open/etc, so check if init is done
:       lda     _sp_is_init
        bne     :+
        sty     tmp10                   ; need to keep the args count incase there's no network
        jsr     _sp_init
        ldy     tmp10                   ; restore y, may be needed if there's no network

:       lda     _sp_network             ; check the network id is not 0
        beq     args_error

        jsr     _sp_clr_payload         ; nuke the payload

        ldy     #$00
        sty     _fn_device_error        ; no error yet

        popax   ptr1                    ; len
        sta     _sp_payload
        stx     _sp_payload+1           ; store length in _sp_payload[0,1]

        popax   ptr2                    ; buffer
        popax   ptr3                    ; use_aux, some commands ignore aux bytes (e.g. read/write)
        jsr     incsp2                  ; devicespec - unused on apple, _sp_network must be set
        popa    tmp6                    ; aux2
        popa    tmp5                    ; aux1
        popa    tmp4                    ; control command

        lda     ptr3                    ; use aux?
        beq     no_aux

        ; yes, use aux
        mwa     #_sp_payload+4, ptr3    ; P3 = payload[4,5]
        mwa     tmp5, _sp_payload+2     ; copy aux into payload[2,3]
        jmp     over

no_aux:
        mwa     #_sp_payload+2, ptr3    ; P3 = payload[2,3]

over:
        lda     ptr2                    ; check if the buffer is set
        ora     ptr2+1
        beq     over_copy               ; it's 0, so skip the copy

        ; there is a buffer, so copy it to sp_payload[2 or 4] depending on use_aux
        pushax  ptr3                    ; dest, either sp_payload+2 or +4
        pushax  ptr2                    ; src (buffer from args)
        setax   ptr1                    ; length
        jsr     _memcpy                 ; trashes p1/2/3. but non are needed anymore

over_copy:
        pusha   _sp_network
        lda     tmp4
        jsr     _sp_control
        jmp     _fn_error               ; process any error from sp_control

.endproc
