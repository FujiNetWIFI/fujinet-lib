                case    on              required for C compatibility
		mcopy   13:orcainclude:m16.orca assembler macros (short, long etc.)
                mcopy   13:ainclude:M16.MiscTool

; int8_t sp_dispatch(uint8_t cmd)
;
; returns any error code from the smart port _sp_dispatch function
sp_dispatch     start

                copy    apple2:src:include:sp.equ

fwdata_ptr      equ 1                   long ptr
error           equ 5
 
                ldy     #SP_PAYLOAD_SIZE+$13  command type offset
                short   m
                sta     [fwdata_ptr],y  store command type
                long    m

; let's invoque FWEntry
                ph8     #0              space for result
                ph2     #0              A register on entry
                ph2     #0              X register on entry
                ph2     #0              Y register on entry
                lda     fwdata_ptr
                clc
                adc     #SP_PAYLOAD_SIZE+$10  JSR offset
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
                sta     error
                tax
                stx     sp_error
                long    i
                plx                     Processor status at exit

                rts

                end

