                case    on              required for C compatibility
		mcopy   13/ainclude/m16.cc
		mcopy   13/orcainclude/m16.orca
                mcopy   13:ainclude:M16.MiscTool

; int8_t sp_dispatch(uint8_t cmd)
;
; returns any error code from the smart port _sp_dispatch function
sp_dispatch     start
                csubroutine 2:cmd,4          4 bytes direct page space
 
fwdata_ptr      equ 1                   32-bit ptr
 
                ldx     sp_fwdata
                stx     fwdata_ptr
                stz     fwdata_ptr+2
                lda     cmd
                ldy     #$40e           dispatch data offset
                sta     [fwdata_ptr],y
                lda     fwdata_ptr
                clc
                adc     #$400           cmdlist offset
                iny                     dispatch data offset + 1
                sta     [fwdata_ptr],y
                short   m
                lda     #$60            RTS opcode
                iny
                iny                     dispatch data offset + 3
                sta     [fwdata_ptr],y
                long    m

; let's invoque FWEntry
                ph8     #0              space for result
                ph2     #0              A register on entry
                ph2     #0              X register on entry
                ph2     #0              Y register on entry
                adc     #$0a            JSR offset = $400 + $A
                pha
                _FWEntry
                short   i
                pla
                tax
                stx     sp_count+1
                pla
                tax
                stx     sp_count
                pla
                tax
                stx     sp_error
                sta     fwdata_ptr
                pla
                long    i

                creturn 2:fwdata_ptr

                end

