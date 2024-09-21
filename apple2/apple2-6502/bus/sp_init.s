        .export         _sp_init

        .import         _sp_dispatch_address
        .import         _sp_get_network_id
        .import         _sp_is_init
        .import         _sp_network

        .import         popa
        .import         return0

        .include        "macros.inc"
        .include        "zp.inc"
        .macpack        cpu

; Find the SmartPort device that has a FujiNet NETWORK adapter on it.
; Really we should search for the FUJI device on it, but historically that was
; hacked into the DISK_0 device for reasons I'll never ever understand.

.proc _sp_init

; ptr1 = base (c701, c601, ...)

.if (.cpu .bitand ::CPU_ISET_65SC02)
        stz     _sp_network
        stz     _sp_is_init
        stz     ptr1
.else
        lda     #$00
        sta     _sp_network
        sta     _sp_is_init
        sta     ptr1
.endif

        ; setup ptr1 to point to C700, it will decrease as we search cards
        ldx     #$c7
        stx     ptr1+1

        ; begin the detect loop
main_loop:
        ldy     #$01

four_loop:
        lda     (ptr1), y               ; Cn00 + y
        cmp     sp_detect_table, y      ; table + y, which has extra bytes to align this loop
        bne     no_match
        iny                             ; we look at Cn01, Cn03, Cn05, Cn07, so increment Y by 2
        iny
        cpy     #$09
        bne     four_loop

matched_four:
        ; get the dispatch offset. ptr1+1 holds the correct Cn value
        lda     #$ff
        sta     ptr1

        ; load $CnFF value, it's the ProDos entry vector, add 3 for SmartPort vector
.if (.cpu .bitand ::CPU_ISET_65SC02)
        lda     (ptr1)
.else
        ldy     #$00
        lda     (ptr1), y
.endif
        ; add 3, then store in dispatch address
        ; we know C is set because the previous cpy
        adc     #$02
        sta     _sp_dispatch_address
        ; x is still Cn
        stx     _sp_dispatch_address+1

check_network:
        ; we set sp_is_init to stop sp_get_network_id from recursing
        lda     #$01
        sta     _sp_is_init
        
        ; store ptr1, and X while we go do stuff
        lda     ptr1
        pha
        lda     ptr1+1
        pha
.if (.cpu .bitand ::CPU_ISET_65SC02)
        phx
.else
        txa
        pha
.endif
        jsr     _sp_get_network_id

        bne     found_network

        ; failed to find a network device on this card, try the next one
        ; first restore the loop state
.if (.cpu .bitand ::CPU_ISET_65SC02)
        plx
.else
        pla
        tax
.endif
        pla
        sta     ptr1+1
        pla
        sta     ptr1

.if (.cpu .bitand ::CPU_ISET_65SC02)
        stz     _sp_is_init
.else
        lda     #$00
        sta     _sp_is_init
.endif

no_match:
        ; decrease ptr1+1 high byte of CnXX
        dex
        stx     ptr1+1
        cpx     #$c0
        bne     main_loop

        ; failed to find a network device on any card
        jmp     return0

found_network:
        ; A contains the sp_network_id already

        ; remove ptr1 and X stored values from stack
.if (.cpu .bitand ::CPU_ISET_65SC02)
        plx
        plx
        plx
.else
        tax             ; save the A value, remove 3 values from stack, then restore A
        pla
        pla
        pla
        txa
.endif
        ldx     #$00
        cmp     #$00    ; force the flags to reflect A reg
        rts
.endproc

.rodata
; intersperse with $ff (any value will do) to allow comparison loop to work. adds 4 bytes here, but less in code
sp_detect_table:        .byte $ff, $20, $ff, $00, $ff, $03, $ff, $00


;; Original C implementation
;
; uint8_t sp_init() {
;     const uint8_t sp_markers[] = {0x20, 0x00, 0x03, 0x00};
;     uint16_t base;
;     uint8_t i;
;     bool match;
;     uint8_t offset;
;     uint16_t dispatch_address;

;     // reset network id and is_init flag, we are going to rescan.
;     sp_network = 0;
;     sp_is_init = 0;

;     for (base = 0xC701; base >= 0xC101; base -= 0x0100) {
;         match = true;
;         for (i = 0; i < 4; i++) {
;             if (read_memory(i * 2, base) != sp_markers[i]) {
;                 match = false;
;                 break;
;             }
;         }

;         if (match) {
;             // If a match is found, calculate the dispatch function address
;             offset = read_memory(0xFE, base);
;             dispatch_address = base + offset + 2;
;             sp_dispatch_address[0] = dispatch_address & 0xFF;
;             sp_dispatch_address[1] = dispatch_address >> 8;

;             // now find and return the network id. it's stored in sp_network after calling sp_get_network_id.
;             // we need to set sp_is_init to 1 to stop sp_get_network_id from calling init again and recursing.
;             sp_is_init = 1;
;             sp_get_network_id();
;             if (sp_network != 0) {
;                 return sp_network;
;             }
;             // it failed to find a network on this SP device, so reset sp_is_init and reloop/exit
;             sp_is_init = 0;
;         }
;     }

;     // no match is found, return 0 for network not found.
;     return 0;
; }