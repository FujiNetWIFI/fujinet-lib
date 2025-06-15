        .export         _clock_get_tz

        .import         _bus
        .import         _fn_error

        .import         popax
        .import         return0

        .include        "device.inc"
        .include        "clock.inc"
        .include        "macros.inc"
        .include        "zp.inc"

; uint8_t clock_get_tz(uint8_t* tz);
_clock_get_tz:
        axinto  tmp_tz_loc              ; save the tz buffer while we get the length of system tz

        ; get the current tz length into "tmp_len"
        mva     #SIO_CLOCK_DEVICE_ID, IO_DCB::ddevic
        mva     #'L', IO_DCB::dcomnd
        mva     #$40, IO_DCB::dstats    ; read
        ldx     #$00
        stx     IO_DCB::dunuse
        stx     IO_DCB::daux1
        stx     IO_DCB::daux2
        stx     IO_DCB::dbythi
        inx                             ; x = 1
        stx     IO_DCB::dunit
        stx     IO_DCB::dbytlo          ; only 1 byte to read 
        inx                             ; x = 2 - as this is a FN non network call, we can keep this low
        stx     IO_DCB::dtimlo
        mwa     #tmp_len, IO_DCB::dbuflo
        jsr     _bus

        ; check if we had a success (dstats holds status result)
        lda     IO_DCB::dstats
        jsr     _fn_error
        beq     ok

        ; error return, we weren't able to find the length of the current timezone
        rts

ok:
        ; fetch "tmp_len" bytes from the FN for the tz string
        mwa     tmp_tz_loc, IO_DCB::dbuflo      ; tz buffer into dbuf
        mva     #'G',       IO_DCB::dcomnd      ; Get system TZ string
        mva     tmp_len,    IO_DCB::dbytlo      ; string length of timezone
        mva     #$40,       IO_DCB::dstats      ; read

        ;; non of these have changed since previous BUS call
        ; ldx     #$00
        ; stx     IO_DCB::dunuse
        ; stx     IO_DCB::daux1
        ; stx     IO_DCB::daux2
        ; stx     IO_DCB::dbythi
        ; inx                            ; x = 1
        ; stx     IO_DCB::dunit
        ; inx                            ; x = 2 - as this is a FN non network call, we can keep this low
        ; stx     IO_DCB::dtimlo
        jsr     _bus
        lda     IO_DCB::dstats
        jmp     _fn_error


.bss
tmp_len:        .res 1
tmp_tz_loc:     .res 2