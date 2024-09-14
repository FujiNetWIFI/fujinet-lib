        .export         _sp_get_modem_id
        .export         _sp_modem_id

        .import         _sp_find_device

        .import         return0
        .import         return1

        .macpack        cpu

sp_find_modem:
        lda     #$15
        jsr     _sp_find_device

        bmi     not_found
        beq     not_found

        sta     _sp_modem_id
        jmp     return1

not_found:
.if (.cpu .bitand ::CPU_ISET_65SC02)
        stz     _sp_modem_id
.else
        lda     #$00
        sta     _sp_modem_id
.endif
        jmp     return0


; uint8_t sp_get_modem_id()
_sp_get_modem_id:
        ldx     #$00                    ; prep the return hi byte for C callers
        lda     _sp_modem_id
        bne     :+                      ; if it's already set, just exit

        ; otherwise we need to try and find it from SP
        jsr     sp_find_modem

        ; return whatever was set in sp_modem
        ldx     #$00
        lda     _sp_modem_id
:       rts

.data
_sp_modem_id:   .byte $00
