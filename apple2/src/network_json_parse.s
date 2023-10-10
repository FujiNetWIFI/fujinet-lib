        .export     _network_json_parse

        .import     _bad_unit
        .import     _fn_device_error
        .import     _fn_error
        .import     _sp_control
        .import     _sp_network
        .import     _sp_payload
        .import     fn_open_mode
        .import     pusha

        .include    "sp.inc"
        .include    "macros.inc"

; uint8_t network_json_parse(char *devicespec);
;
.proc _network_json_parse
        ; devicespec is in A/X but ignored for apple2, as there's only 1 device
        ldy     #$00
        sty     _fn_device_error

        lda     _sp_network     ; get network id
        beq     no_network

        ; -----------------------------------------------------------------
        ; JSON channel mode
        jsr     pusha           ; push network unit into stack to be read by sp_control for JSON channel mode call

        lda     #$01
        sta     _sp_payload
        sta     _sp_payload+2   ; JSON mode
        lda     #$00
        sta     _sp_payload+1
        lda     #$fc            ; set json channel mode
        jsr     _sp_control
        bne     error

        ; -----------------------------------------------------------------
        ; JSON parse
        lda     _sp_network
        jsr     pusha
        lda     #'P'
        jsr     _sp_control
        ; fall through to the error handler to return appropriate code, OK is handled too

error:
        jmp     _fn_error

no_network:
        ; no params to remove from stack, just set return
        jmp     _bad_unit
.endproc
