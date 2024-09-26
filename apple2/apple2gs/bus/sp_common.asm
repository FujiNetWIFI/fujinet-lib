                case    on              required for C compatibility
		mcopy   13/orcainclude/m16.orca assembler macros (short, long etc.)

; set X to the unit number
; set A to the status or control code

; CMDLIST       Parameter count (set by sp_status and sp_control)
;               Unit number
;               Payload pointer (low byte)
;               Payload pointer (high byte)
;               Status/control code

sp_common_params start

fwdata_ptr      equ 1                   32-bit ptr

                copy    apple2:src:include:sp.equ

                pha
                phx
                ldx     sp_fwdata
                stx     fwdata_ptr
                stz     fwdata_ptr+2
                ldy     #SP_PAYLOAD_SIZE+1      cmdlist offset + 1
                pla                             unit number
                sta     [fwdata_ptr],y          store unit number
                txa                             set payload pointer to sp_fwdata from X
                iny                             cmdlist offset + 2
                sta     [fwdata_ptr],y          set payload pointer
                pla                             status/control code
                iny
                iny                             cmdlist offset + 4
                short   m
                sta     [fwdata_ptr],y          store status/control code
                long    m

                rts

                end

                