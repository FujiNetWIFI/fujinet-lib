        .export         _sp_find_cpm
        .export         _sp_get_cpm_id
        .export         _sp_cpm_id

        .import         _sp_find_device

        .import         return0
        .import         return1

        .macpack        cpu

; bool sp_find_cpm()
_sp_find_cpm:
        lda     #$12
        jsr     _sp_find_device

        bmi     not_found
        beq     not_found

        sta     _sp_cpm_id
        jmp     return1

not_found:

.if (.cpu .bitand ::CPU_ISET_65SC02)
        stz     _sp_cpm_id
.else
        lda     #$00
        sta     _sp_cpm_id
.endif
        jmp     return0


; uint8_t sp_get_cpm_id()
_sp_get_cpm_id:
        ldx     #$00                    ; prep the return hi byte for C callers
        lda     _sp_cpm_id
        beq     _sp_find_cpm

        ; A contains the ID > 0, X is 0, so just return
        rts

.data
_sp_cpm_id:   .byte $00
