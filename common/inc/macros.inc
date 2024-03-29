; various macros to make asm easier to read

.macro  mwa    arg1, arg2
    .if (.match (.left (1, {arg1}), #))
        ; arg1 is immediate mode
        lda     #<(.right (.tcount ({arg1})-1, {arg1}))
        sta     arg2
        lda     #>(.right (.tcount ({arg1})-1, {arg1}))
        ; have to use "1+..." to stop assembler thinking it's an addressing mode
        sta     1+(arg2)
    .else
        lda     arg1
        sta     arg2
        lda     1+(arg1)
        sta     1+(arg2)
    .endif
.endmacro

.macro mway arg1, arg2
    .if (.match (.left (1, {arg1}), #))
        lda     #<(.right (.tcount ({arg1})-1, {arg1}))
        sta     arg2
        iny
        lda     #>(.right (.tcount ({arg1})-1, {arg1}))
        sta     arg2
    .else
        lda     arg1
        sta     arg2
        iny
        lda     1+(arg1)
        sta     arg2
    .endif
.endmacro

.macro  mva arg1, arg2
    ; this could be immediate or absolute, doesn't matter.
    ; commas are arg separators, so `mva #$00, {($80),y}` is how to use this with addressing modes
    lda     arg1
    sta     arg2
.endmacro

.macro cpw arg1, arg2
    .local s1, s2
    .if (.match (.left (1, {arg2}), #))
        lda     1+(arg1)
        cmp     #>(.right (.tcount ({arg2})-1, {arg2}))
        bne     s1
        lda     arg1
        cmp     #<(.right (.tcount ({arg2})-1, {arg2}))
@s1:
    .else
        lda     1+(arg1)
        cmp     1+(arg2)
        bne     s2
        lda     arg1
        cmp     arg2
@s2:
    .endif
.endmacro

; use CA65 function calling conventions, passing values into SP via pushax
.macro pushax arg1
    .if (.match (.left (1, {arg1}), #))
        ldx     #>(.right (.tcount ({arg1})-1, {arg1}))
        lda     #<(.right (.tcount ({arg1})-1, {arg1}))
        jsr     pushax
    .else
        ldx     1+(arg1)
        lda     arg1
        jsr     pushax
    .endif
.endmacro

; push arg1 into SP from A
.macro pusha arg1
    lda     arg1
    jsr     pusha
.endmacro

; same as pushax, but without the push to SP, preparing the args directly for consumption
; always set X first, then A so flags are set based on A
.macro setax arg1
    .if (.match (.left (1, {arg1}), #))
        ldx     #>(.right (.tcount ({arg1})-1, {arg1}))
        lda     #<(.right (.tcount ({arg1})-1, {arg1}))
    .else
        ldx     1+(arg1)
        lda     arg1
    .endif
.endmacro

.macro popax arg1
    jsr     popax
    stx     1+(arg1)
    sta     arg1
.endmacro

.macro popa arg1
    jsr     popa
    sta     arg1
.endmacro

.macro axinto arg1
    stx     1+(arg1)
    sta     arg1
.endmacro

; adw: adds word values together, or immediate bytes.
; adw src, #$40
; adw src, #$4000
; adw a, b
; adw src, #$40, dst
; adw src, #$4000, dst
; adw a, b, c
.macro adw arg1, arg2, arg3
    .local @l1_a, @l1_b
    ; ensure arg1/arg3 don't have immediate values. They are always locations.
    .if (.match (.left (1, {arg1}), #))
        .error "adw: arg1 cannot be literal"
    .endif
    .if (.match (.left (1, {arg3}), #))
        .error "adw: arg3 cannot be literal"
    .endif

    clc
    .if (.match (.left (1, {arg2}), #))
        ; immediate value in arg2
        lda     arg1
        adc     #<(.right (.tcount ({arg2})-1, {arg2}))
        .ifnblank arg3
            sta     arg3
        .else
            sta     arg1
        .endif

        .if .hibyte (.right (.tcount ({arg2})-1, {arg2})) = 0
            .ifnblank arg3
                ; must store high byte of arg1 in arg3 still, and adjust if carry set
                lda 1+(arg1)
                sta 1+(arg3)
                bcc @l1_a
                inc 1+(arg3)
@l1_a:
            .else
                ; the literal was only a byte, so only need to check carry
                bcc @l1_b
                inc 1+(arg1)
@l1_b:
            .endif
        .else
            ; full add of 2nd byte of constant
            lda     1+(arg1)
            adc     #>(.right (.tcount ({arg2})-1, {arg2}))
            .ifnblank arg3
                sta     1+(arg3)
            .else
                sta     1+(arg1)
            .endif
        .endif
    .else
        lda     arg1
        adc     arg2
        .ifnblank arg3
            sta     arg3
        .else
            sta     arg1
        .endif
        lda     1+(arg1)
        adc     1+(arg2)
        .ifnblank arg3
            sta     1+(arg3)
        .else
            sta     1+(arg1)
        .endif
    .endif
.endmacro

; sbw: subtracts word values together, or immediate bytes.
; complement of adw
.macro sbw arg1, arg2, arg3
    .local @l1_a, @l1_b
    ; ensure arg1/arg3 don't have immediate values. They are always locations.
    .if (.match (.left (1, {arg1}), #))
        .error "sbw: arg1 cannot be literal"
    .endif
    .if (.match (.left (1, {arg3}), #))
        .error "sbw: arg3 cannot be literal"
    .endif

    sec
    .if (.match (.left (1, {arg2}), #))
        ; immediate value in arg2
        lda     arg1
        sbc     #<(.right (.tcount ({arg2})-1, {arg2}))
        .ifnblank arg3
            sta     arg3
        .else
            sta     arg1
        .endif

        .if .hibyte (.right (.tcount ({arg2})-1, {arg2})) = 0
            .ifnblank arg3
                ; must store high byte of arg1 in arg3 still, and adjust if carry set
                lda 1+(arg1)
                sta 1+(arg3)
                bcs @l1_a
                dec 1+(arg3)
@l1_a:
            .else
                ; the literal was only a byte, so only need to check carry
                bcs @l1_b
                dec 1+(arg1)
@l1_b:
            .endif
        .else
            ; full add of 2nd byte of constant
            lda     1+(arg1)
            sbc     #>(.right (.tcount ({arg2})-1, {arg2}))
            .ifnblank arg3
                sta     1+(arg3)
            .else
                sta     1+(arg1)
            .endif
        .endif
    .else
        lda     arg1
        sbc     arg2
        .ifnblank arg3
            sta     arg3
        .else
            sta     arg1
        .endif
        lda     1+(arg1)
        sbc     1+(arg2)
        .ifnblank arg3
            sta     1+(arg3)
        .else
            sta     1+(arg1)
        .endif
    .endif
.endmacro

; adds byte1 to word arg1 dealing with carry, allow byte1 to be a
; so we can directly add accumulator to WORD
.macro adw1 arg1, byte1
    .local @l1
    clc
    .if (.match ({byte1}, a))
        adc arg1
        sta arg1
    .else
        lda arg1
        adc byte1
        sta arg1
    .endif
    bcc @l1
    inc 1+(arg1)
@l1:
.endmacro

; subtracts byte1 from word arg1 dealing with carry
.macro sbw1 arg1, byte1
    .local @l1
    sec
    lda arg1
    sbc byte1
    sta arg1
    bcs @l1
    dec 1+(arg1)
@l1:
.endmacro