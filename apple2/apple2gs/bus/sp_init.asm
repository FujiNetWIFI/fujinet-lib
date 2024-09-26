                case    on              required for C compatibility
                mcopy   13:ainclude:m16.cc
                mcopy   13/orcainclude/m16.orca assembler macros (short, long etc.)
                mcopy   13:ainclude:M16.MiscTool
                mcopy   13:ainclude:M16.Memory

sp_init         start
                csubroutine ,8          8 bytes direct page space

                copy    apple2:src:include:sp.equ

; ptr1 = base (c701, c601, ...), long addressing
ptr1            equ 1                   32-bit ptr
fwdata_ptr      equ 5

                _MTStartup              startup Miscelaneous Tools
                pha                     space for result
                pha
                ph4 #SP_PAYLOAD_SIZE+$20  size of block to create
                pha
                _MMStartup              startup Memory Manager
                pla
                ora #$0f00              OR in an arbitrary auxiliary ID
                sta MyID                save user ID
                pha                     user ID
                ph2 #$C001              fixed, locked, use specified bank
                ph4 #0                  (specify bank 0)
                _NewHandle
                
; NEED TO ADD: check NewHandle error and check ProDOS 16 vs 8 (see c code in sp.c pre-lib)

; dereference pointer and store to sp_fwdata and direct page
                pla
                sta ptr1
                pla
                sta ptr1+2
                lda [ptr1]
                sta sp_fwdata
                sta fwdata_ptr
                stz fwdata_ptr+2

; setup 
                short m                 8-bit accumulator, 16-bit index registers
                stz sp_network
                stz sp_is_init

; setup ptr1 to point to C700, it will decrease as we search cards
                lda #$c7
                sta ptr1+1
                stz ptr1+2              ptr1 points to databank 0

; begin the detect loop
main_loop       stz ptr1

                ldy #$01
four_loop       lda [ptr1],y            Cn00 + y
                cmp sp_detect_table,y   table + y, which has extra bytes to align this loop
                bne no_match
                iny                     we look at Cn01, Cn03, Cn05, Cn07, so increment Y by 2
                iny
                cpy #$09
                bne four_loop

; get the dispatch offset. ptr1+1 holds the correct Cn value
matched_four    lda #$ff
                sta ptr1

; here we construct the SmartPort call in bank $00
; call starts at fw_data + SP_PAYLOAD_SIZE + $10
; SP_CALL   JSR DISPATCH    Call SmartPort command dispatcher
;           DFB CMDNUM      Will be set by sp_status and sp_control
;           DW  CMDLIST     Word pointer to the parameter list in bank $00
;           RTS             Carry is set on error
                ldy #SP_PAYLOAD_SIZE+$10  JSR sp dispatch address offset
                lda #$20                opcode for JSR
                sta [fwdata_ptr],y
; load $CnFF value, it's the ProDos entry vector, add 3 for SmartPort vector
                lda [ptr1]
; add 3, then store in dispatch address
; clc - we know C is set because the previous cpy sets it in the comparison, so we can just add 2 here instead of 3 and save a byte
                adc #$02
                iny
                sta [fwdata_ptr],y
                lda  ptr1+1
                iny
                sta [fwdata_ptr],y
                long m
                lda fwdata_ptr
                clc
                adc #SP_PAYLOAD_SIZE    accumulator points to CMDLIST
                iny
                iny
                sta [fwdata_ptr],y
                short m
                lda #$60                opcode for RTS
                iny
                iny
                sta [fwdata_ptr],y

; we set sp_is_init to stop sp_get_network_id from recursing
check_network   lda #$01
                sta sp_is_init
                
                long m
                jsl sp_get_network_id
                bne found_network
                short m  

                stz sp_is_init

; decrease ptr1+1 high byte of CnXX
no_match        dec ptr1+1
                lda ptr1+1
                cmp #$c0
                bne main_loop

; failed to find a network device on any card
                long m
                lda #0

found_network   sta ptr1
                creturn 2:ptr1

; intersperse with $ff (any value will do) to allow comparison loop to work. adds 4 bytes here, but less in code
sp_detect_table dc      h'ff 20 ff 00 ff 03 ff 00'

                end
