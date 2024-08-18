                case    on              required for C compatibility
                mcopy   13:ainclude:m16.cc
                mcopy   13:orcainclude:m16.orca
                mcopy   13:ainclude:M16.MiscTool
                mcopy   13:ainclude:M16.Memory

; C version 246 bytes, this is 104 or 110 bytes (65c02 vs 6502) as hand written asm

sp_init         start
                csubroutine ,10         10 bytes direct page space to store ptr1 & ptr2
                short m                 8-bit accumulator, 16-bit index registers

ptr1            equ 1                   32-bit ptr
ptr2            equ 5                   16-bit ptr
fwdata_ptr      equ 7

; ptr1 = base (c701, c601, ...), long addressing
; ptr2 = sp_detect_table, absolute addressing

                phb
	            phk     	            set code and data bank to same bank
	            plb		                so can use absolute addressing

                _MTStartup              startup Miscelaneous Tools
                ph2 #0                  16 bits space for result
                _MMStartup              startup Memory Manager
                ply
                sty MyID                save user ID
                ph4 #0                  32 bits space for result
                ph4 #$500               size of block to create
                phy                     user ID
                ph2 #$4001              we want a fixed block in bank 0               
                ph4 #0                  bank 0
                _NewHandle

; NEED TO ADD: check handle<>0 and check ProDOS 16 vs 8 (see c code in sp.c pre-lib)

; dereference pointer and store to sp_fwdata and direct page
                ply
                sty ptr1
                ply
                sty ptr1+2
                long m
                lda [ptr1]
                sta sp_fwdata
                sta fwdata_ptr
                short m
                stz fwdata_ptr+2

; setup 

                stz sp_network
                stz sp_is_init

; setup ptr1 to point to C700, it will decrease as we search cards
                ldy #$c700
                sty ptr1
                stz ptr1+2              ptr1 points to databank 0

; begin the detect loop
main_loop       ldy #sp_detect_table
                sty ptr2

                ldy #$01
four_loop       lda [ptr1],y            Cn00 + y
                cmp (ptr2),y            table + y, which has extra bytes to align this loop
                bne no_match
                iny                     we look at Cn01, Cn03, Cn05, Cn07, so increment Y by 2
                iny
                cpy #$09
                bne four_loop

; get the dispatch offset. ptr1+1 holds the correct Cn value
matched_four    lda #$ff
                sta ptr1

; store $20 (JSR) in fwdata memory address $40A
                ldy #$40a               JSR sp dispatch address offset
                lda #$20                opcode for JSR
                sta [fwdata_ptr],y
; load $CnFF value, it's the ProDos entry vector, add 3 for SmartPort vector
                lda [ptr1]
; add 3, then store in dispatch address
; clc - we know C is set because the previous cpy sets it in the comparison, so we can just add 2 here instead of 3 and save a byte
                adc #$02
                iny                     JSR sp dispatch address offset + 1
                sta [fwdata_ptr],y
                lda  ptr1+1
                iny                     JSR sp dispatch address offset + 2
                sta [fwdata_ptr],y

; we set sp_is_init to stop sp_get_network_id from recursing
check_network   lda #$01
                sta sp_is_init

                long m
                jsl sp_get_network_id
                short m  

                bne found_network

                stz sp_is_init

; decrease ptr1+1 high byte of CnXX
no_match        dec     ptr1+1
                lda     ptr1+1
                cmp     #$c0
                bne     main_loop

; failed to find a network device on any card
                lda     #0

found_network   sta     ptr1
                plb
                creturn 2:ptr1

; intersperse with $ff (any value will do) to allow comparison loop to work. adds 4 bytes here, but less in code
sp_detect_table dc      h'ff 20 ff 00 ff 03 ff 00'

                end

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
