                case    on              required for C compatibility
                mcopy   13/ainclude/m16.cc      csubroutine & creturn macros
                mcopy   13/orcainclude/m16.orca assembler macros (short, long etc.)

sp_status start
                csubroutine (2:dest,2:statcode),6       6 bytes direct page space

                copy    apple2:src:include:sp.equ

fwdata_ptr      equ 1                   32-bit ptr
error           equ 5

                ldx     dest
                lda     statcode
                jsr     sp_common_params
                lda     #SP_STATUS_PARAM_COUNT
                ldy     #SP_PAYLOAD_SIZE        cmdlist offset
                short   m
                sta     [fwdata_ptr],y
                long    m
                lda     #SP_CMD_STATUS
                jsr     sp_dispatch

; copy returned bytes from bank 0 to sp_payload
                ldx     fwdata_ptr
                ldy     #sp_payload
                lda     #SP_PAYLOAD_SIZE-1
                mvn     0,sp_payload

                creturn 2:error

                end

