        .export     _spn_status

        .import     _spn_count
        .import     _spn_cmdlist
        .import     _spn_dispatch
        .import     _spn_error
        .import     _spn_payload
        .import     popa

        .include    "sp.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; int8_t _spn_status(uint8_t dest, uint8_t statcode)
;
; call smart port for status using given code for given destination
; this changes _spn_payload, and _spn_count
; returns any error code from dispatch call
.proc _spn_status
        sta     tmp1                    ; store statcode until we need it
        popa    tmp2                    ; store dest, popa trashes y, so need to store it now instead of later

        ; reset error
        mva     #$00, _spn_error

        ; -------------------------------------------------------
        ; setup cmdlist table
        mwa     #_spn_cmdlist, ptr1
        ldy     #$00

        ; status params count in cmdlist[0]
        mva     #SP_STATUS_PARAM_COUNT, {(ptr1), y}
        iny

        ; dest param in cmdlist[1]
        mva     tmp2, {(ptr1), y}
        iny

        ; payload buffer location in cmdlist[2,3]
        mway    #_spn_payload, {(ptr1), y}
        iny

        ; finally the statcode in cmdlist[4]
        mva     tmp1, {(ptr1), y}

        ; -------------------------------------------------------
        ; set dispatch data
        ldy     #$00
        mwa     #dispatch_data, ptr2

        ; set command
        mva     #SP_CMD_STATUS, {(ptr2), y}
        iny

        ; set cmdlist (ptr1) location
        mway    ptr1, {(ptr2), y}

; -------------------------------------------------------------
; DISPATCH CALL

        jsr     do_dispatch
dispatch_data:
        .byte   $00, $00, $00

        ; X/Y contains count (of bytes transferred), A contains error
        stx     _spn_count
        sty     _spn_count+1
        sta     _spn_error

        ldx     #$00
        lda     _spn_error              ; forces Z flag based on error value
        rts

do_dispatch:
        mwa     _spn_dispatch, ptr1
        jmp     (ptr1)
        ; rts from the call will return to above caller + 3 bytes, as dispatcher affects return address

.endproc
