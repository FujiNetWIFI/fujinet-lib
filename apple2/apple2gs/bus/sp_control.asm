                case    on              required for C compatibility
                mcopy   13/ainclude/m16.cc      csubroutine & creturn macros
                mcopy   13/orcainclude/m16.orca assembler macros (short, long etc.)

sp_control      start
                csubroutine (2:dest,2:ctrlcode),6       6 bytes direct page space

                copy    apple2:src:include:sp.equ

fwdata_ptr      equ 1                   32-bit ptr
error           equ 5

                ldx     dest
                lda     ctrlcode
                jsr     sp_common_params

; prior to calling FWEntry, copy sp_payload to bank 0
; data bank register will be set to 0 (the destination bank) by MVN
; we need to save and restore it from stack
                phb                     
                ldx     #sp_payload
                ldy     fwdata_ptr
                lda     #SP_PAYLOAD_SIZE-1
                mvn     sp_payload,0
                plb

                lda     #SP_CONTROL_PARAM_COUNT
                short   m
                sta     [fwdata_ptr],y
                long    m
                lda     #SP_CMD_CONTROL
                jsr     sp_dispatch

                creturn 2:error

                end

