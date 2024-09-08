                case    on              required for C compatibility
		mcopy   13/orcainclude/m16.orca
                mcopy   13:ainclude:M16.MiscTool

; int8_t sp_dispatch(uint8_t cmd)
;
; returns any error code from the smart port _sp_dispatch function
sp_dispatch     start
 
fwdata_ptr      equ 1                   32-bit ptr
 
                ldy     #$40d           dispatch data offset
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
                lda     fwdata_ptr
                clc
                adc     #$40a           JSR offset = $40A
                pha
                _FWEntry
                short   i
                pla                     Y register at exit
                tax
                stx     sp_count+1
                pla                     X register at exit
                tax
                stx     sp_count
                pla                     Accumulator at exit
                tax
                stx     sp_error
                long    i
                plx                     Processor status at exit

                rts

                end

