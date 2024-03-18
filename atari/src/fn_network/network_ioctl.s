        .export     _network_ioctl

        .import     _bus
        .import     _fn_device_error
        .import     _bus_status
        .import     _network_unit
        .import     addysp
        .import     popa
        .import     popax
        .import     return1

        .include    "fujinet-network.inc"
        .include    "device.inc"
        .include    "macros.inc"

; uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, ...);
;
; for atari this must always be called with:
; uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, char* devicespec, [uint8_t dstats, uint16_t dbuf, uint16_t dbyt]);
;
; CC65 uses 2 bytes for every variadic parameter, even if they are uint8_t (from cl65 output), so each param
; should be done with popax.
;
; ASSEMBLY CALLERS MUST SET Y TO 11 ($b) TO EMULATE C CALL WITH VARARGS
.proc _network_ioctl
        cpy     #$0b                    ; 5 for standard args, 3 extra args at 2 bytes each = 5 + 6 = 11 bytes on stack. if not, we have been called incorrectly
        bne     @args_error

        ldy     #$00
        sty     _fn_device_error

        ; we have every DCB parameter on stack
        jsr     popax                   ; dbyt
        axinto  IO_DCB::dbytlo

        jsr     popax                   ; dbuf
        axinto  IO_DCB::dbuflo

        jsr     popax                   ; dstats, ignore X. cc65 does 2 bytes on stack for all varargs, we only need 1 here
        sta     IO_DCB::dstats

        jsr     popax                   ; devicespec, we can only get the UNIT out of this
        jsr     _network_unit
        sta     IO_DCB::dunit

        jsr     popa                    ; aux2
        sta     IO_DCB::daux2

        jsr     popa                    ; aux1
        sta     IO_DCB::daux1

        jsr     popa                    ; cmd
        sta     IO_DCB::dcomnd

        mva     #$71, IO_DCB::ddevic    ; device
        mva     #$0f, IO_DCB::dtimlo    ; timeout

        jsr     _bus
        lda     IO_DCB::dunit
        jmp     _bus_status              ; set return to the status

@args_error:
        ; increase SP by Y to clear the params we received, and return error
        jsr     addysp
        ldx     #$00
        stx     _fn_device_error        ; no device error, it's just bad args
        lda     #FN_ERR_BAD_CMD
        rts

.endproc
