        .export         _sp_find_fuji
        .export         _sp_get_fuji_id
        .export         _sp_fuji_id

        .import         _sp_find_device

        .import         return0
        .import         return1

        .macpack        cpu

; bool sp_find_fuji()
_sp_find_fuji:
        lda     #$10
        jsr     _sp_find_device

        bmi     not_found
        beq     not_found

        sta     _sp_fuji_id
        jmp     return1

not_found:

.if (.cpu .bitand ::CPU_ISET_65SC02)
        stz     _sp_fuji_id
.else
        lda     #$00
        sta     _sp_fuji_id
.endif
        jmp     return0


; uint8_t sp_get_fuji_id()
_sp_get_fuji_id:
        ldx     #$00                    ; prep the return hi byte for C callers
        lda     _sp_fuji_id
        beq     _sp_find_fuji

        ; A contains the ID > 0, X is 0, so just return
        rts

.data
_sp_fuji_id:   .byte $00
