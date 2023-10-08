        .export     _network_close

        .import     _bad_unit
        .import     _fn_error
        .import     _sp_control
        .import     _sp_network
        .import     pusha

; uint8_t network_close(char* devicespec);
;
; returns 0 for ok, error code from control call for anything else.
; returns 1 if the sp_network isn't set, e.g. calling before calling _network_open.
.proc _network_close

        ; At the moment, we can't handle multiple device specs at the same time on apple, so the
        ; devicespec parameter is ignored.
        ; so we just close the 1 network device we know about.

        ; TODO: revisit this when there are multiple network devices for multiple device specs.

        lda     _sp_network     ; get network id
        beq     no_network

        jsr     pusha           ; push network unit into stack to be read by sp_control
        lda     #'C'            ; close
        jsr     _sp_control

        ; setting sp_network to 0, as it's checked in various other functions
        ldy     #$00
        sty     _sp_network

        ; convert to fujinet-network error
        jmp     _fn_error

no_network:
        jmp     _bad_unit
.endproc
