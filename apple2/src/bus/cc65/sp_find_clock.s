        .export         _sp_find_clock
        .export         _sp_get_clock_id
        .export         _sp_clock_id

        .import         _sp_find_device

        .import         return0
        .import         return1

        .macpack        cpu

; bool sp_find_clock()
_sp_find_clock:
        lda     #$13
        jsr     _sp_find_device

        bmi     not_found
        beq     not_found

        sta     _sp_clock_id
        jmp     return1

not_found:

.if (.cpu .bitand ::CPU_ISET_65SC02)
        stz     _sp_clock_id
.else
        lda     #$00
        sta     _sp_clock_id
.endif
        jmp     return0


; uint8_t sp_get_clock_id()
_sp_get_clock_id:
        ldx     #$00                    ; prep the return hi byte for C callers
        lda     _sp_clock_id
        beq     _sp_find_clock

        ; A contains the ID > 0, X is 0, so just return
        rts

.data
_sp_clock_id:   .byte $00
