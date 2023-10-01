        .export     _network_close

        .import     _fn_error
        .import     _sp_control
        .import     _sp_network
        .import     pusha
        .import     return1

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
        pha                     ; save return value, x is also affected, but we don't touch that before return

        ; setting sp_network to 0, as it's checked in various other functions
        lda     #$00
        sta     _sp_network

        pla
        ; convert to fujinet-network error
        jmp     _fn_error

no_network:
        jmp     return1
.endproc
