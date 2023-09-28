        .export     spn_setup

        .import     _spn_cmdlist
        .import     _spn_dispatch
        .import     _spn_error
        .import     _spn_payload

        .import     popa

        .include    "macros.inc"
        .include    "zp.inc"


; common internal routine used by spn_control and spn_status as they use 4 params
; tmp1/2 hold dest/<code>
.proc spn_setup
        sta     tmp3            ; param count
        popa    tmp4            ; command

        ; reset error
        mva     #$00, _spn_error

        ; -------------------------------------------------------
        ; setup cmdlist table
        mwa     #_spn_cmdlist, ptr1
        ldy     #$00

        ; params count in cmdlist[0]
        mva     tmp3, {(ptr1), y}
        iny

        ; dest param in cmdlist[1]
        mva     tmp2, {(ptr1), y}
        iny

        ; payload buffer location in cmdlist[2,3]
        mway    #_spn_payload, {(ptr1), y}
        iny

        ; finally the ctrlcode in cmdlist[4]
        mva     tmp1, {(ptr1), y}

        ; -------------------------------------------------------
        ; set dispatch data
        ldy     #$00
        mwa     #dispatch_data, ptr2

        ; set command
        mva     tmp4, {(ptr2), y}
        iny

        ; set cmdlist (ptr1) location
        mway    ptr1, {(ptr2), y}

; -------------------------------------------------------------
; DISPATCH CALL

        jsr     do_dispatch
dispatch_data:
        .byte   $00, $00, $00

        rts

do_dispatch:
        mwa     _spn_dispatch, ptr1
        jmp     (ptr1)
        ; rts from the call will return to above caller + 3 bytes, as dispatcher affects return address

.endproc