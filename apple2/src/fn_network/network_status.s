        .export     _network_status
        .export     _network_status_no_clr

        .import     _bad_unit
        .import     _fn_device_error
        .import     _fn_error
        .import     _sp_clr_payload
        .import     _sp_network
        .import     _sp_payload
        .import     _sp_status
        .import     incsp2
        .import     incsp6
        .import     popax
        .import     pusha
        .import     pushax

        .include    "fujinet-network.inc"
        .include    "macros.inc"
        .include    "sp.inc"
        .include    "zp.inc"

; uint8_t network_status(char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
;
_network_status:
        ; save A/X so we can restore them after clearing payload
        pha
        txa
        pha
        jsr     _sp_clr_payload     ; calls bzero, so trashes p1/2/3
        pla
        tax
        pla
        ; drop into no_clr version

_network_status_no_clr:
        axinto  ptr4            ; save err location

        ldy     #$00
        sty     _fn_device_error

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

        ldy     #$00

        ; process the device error
        lda     _sp_payload+3
        sta     (ptr4), y       ; *err = sp_payload[3]

        ; process the connection status param
        popax   ptr4
        lda     _sp_payload+2
        sta     (ptr4), y       ; *c = sp_payload[2]

        ; process the bytes waiting (bw) param
        popax   ptr4
        mway    _sp_payload, {(ptr4), y}

        ; remove the devicespec parameter from stack, it isn't used
        jsr     incsp2

        ; reload any error from sp_status and deal with it
        pla
        jmp     _fn_error
