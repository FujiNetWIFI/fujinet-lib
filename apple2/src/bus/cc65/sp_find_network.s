        .export         _sp_get_network_id
        .export         _sp_network

        .import         _sp_find_device

        .import         return0
        .import         return1

        .macpack        cpu

sp_find_network:
        lda     #$11
        jsr     _sp_find_device

        bmi     not_found
        beq     not_found

        sta     _sp_network
        jmp     return1

not_found:
.if (.cpu .bitand ::CPU_ISET_65SC02)
        stz     _sp_network
.else
        lda     #$00
        sta     _sp_network
.endif
        jmp     return0


; uint8_t sp_get_network_id()
_sp_get_network_id:
        ldx     #$00                    ; prep the return hi byte for C callers
        lda     _sp_network
        bne     :+                      ; if it's already set, just exit

        ; otherwise we need to try and find it from SP
        jsr     sp_find_network

        ; return whatever was set in sp_network
        ldx     #$00
        lda     _sp_network
:       rts

.data
_sp_network:    .byte $00
