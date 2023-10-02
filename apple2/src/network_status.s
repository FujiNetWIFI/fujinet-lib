        .export     _network_status

        .import     _bad_unit
        .import     _fn_device_error
        .import     _fn_error
        .import     _sp_network
        .import     _sp_payload
        .import     _sp_status
        .import     incsp2
        .import     incsp6
        .import     popax
        .import     pusha

        .include    "fujinet-network.inc"
        .include    "macros.inc"
        .include    "sp.inc"
        .include    "zp.inc"

; uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
;
.proc _network_status
        axinto  ptr1            ; save err location

        ; apple currently does nothing with the devicespec except during open.
        ; where it sets the network device id in _sp_network

        lda     _sp_network     ; get network id
        bne     have_network

        ; remove the function args we didn't read from the stack, save the real error, and return bad command
        jsr     incsp6
        jmp     _bad_unit

have_network:
        jsr     pusha           ; network unit
        lda     #'S'            ; S = network status
        jsr     _sp_status
        pha                     ; save the error until we've dealt with the function args

        ldy     #$00            ; setup the zp ptr index offset

        ; process the err value
        lda     ptr1
        ora     ptr1+1
        beq     @no_err_param

        lda     _sp_payload+3
        sta     (ptr1), y       ; *err = sp_payload[3]

@no_err_param:
        ; process the connection status param
        popax   ptr1
        ora     ptr1+1
        beq     @no_conn_param

        lda     _sp_payload+2
        sta     (ptr1), y       ; *c = sp_payload[2]

@no_conn_param:
        ; process the bytes waiting (bw) param
        popax   ptr1
        ora     ptr1+1
        beq     @no_bw_param

        ; save the 2 bytes at sp_payload[0,1] into address pointed to by bw
        mway    _sp_payload, {(ptr1), y}

@no_bw_param:
        ; remove the devicespec parameter from stack, it isn't used
        jsr     incsp2

        ; reload any error from sp_status and deal with it
        pla
        jmp     _fn_error

.endproc
