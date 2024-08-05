        .export     _network_close

        .import     _bad_unit
        .import     _fn_device_error
        .import     _fn_error
        .import     _network_unit
        .import     _sp_clr_payload
        .import     _sp_control_nw
        .import     _sp_network
        .import     _sp_nw_unit
        .import     pusha

; uint8_t network_close(char* devicespec);
;
; returns 0 for ok, error code from control call for anything else.
; returns 1 if the sp_network isn't set, e.g. calling before calling _network_open.
.proc _network_close
        ; set _sp_nw_unit to the Nx: value, A/X already hold devicespec pointer
        jsr     _network_unit
        sta     _sp_nw_unit

        jsr     _sp_clr_payload

        ldy     #$00
        sty     _fn_device_error

        lda     _sp_network     ; get network id
        beq     no_network

        jsr     pusha           ; push network unit into stack to be read by sp_control
        lda     #'C'            ; close
        jsr     _sp_control_nw

        ; convert to fujinet-network error
        jmp     _fn_error

no_network:
        jmp     _bad_unit
.endproc
