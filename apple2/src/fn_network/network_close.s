        .export     _network_close

        .import     _bad_unit
        .import     _fn_device_error
        .import     _fn_error
        .import     _sp_clr_payload
        .import     _sp_control
        .import     _sp_network
        .import     pusha

; uint8_t network_close(char* devicespec);
;
; returns 0 for ok, error code from control call for anything else.
; returns 1 if the sp_network isn't set, e.g. calling before calling _network_open.
.proc _network_close
        ; At the moment, we can't handle multiple device specs at the same time on apple, so the
        ; devicespec parameter is ignored (it's previously been read in "open" and unit was saved in _sp_network).
        ; so we just close the 1 network device we know about.
        ; TODO: revisit this when there are multiple network devices for multiple device specs.

        ; if params are honoured, they will have to use them before this call which trashes a/x, and p1/2/3
        jsr     _sp_clr_payload

        ldy     #$00
        sty     _fn_device_error

        lda     _sp_network     ; get network id
        beq     no_network

        jsr     pusha           ; push network unit into stack to be read by sp_control
        lda     #'C'            ; close
        jsr     _sp_control

        ; convert to fujinet-network error
        jmp     _fn_error

no_network:
        jmp     _bad_unit
.endproc
