                case    on              required for C compatibility
		mcopy   13/ainclude/m16.cc      csubroutine & creturn macros
		mcopy   13/orcainclude/m16.orca assembler macros (short, long etc.)

sp_status start
        csubroutine (2:dest,2:statcode),4       4 bytes direct page space

        copy    apple2:src:include:sp.equ

fwdata_ptr      equ 1                   32-bit ptr

        ldx     dest
        lda     statcode
        jsr     sp_common_params
        lda     #SP_STATUS_PARAM_COUNT
        ldy     #$400                   cmdlist offset
        short   m
        sta     [fwdata_ptr],y
        long    m
        lda     #SP_CMD_STATUS
        jsr     sp_dispatch
        pha

; copy returned bytes from bank 0 to sp_payload
        ldy     #0
loop1   lda     [fwdata_ptr],y
        sta     sp_payload,y
        iny
        iny
        cpy     #SP_PAYLOAD_SIZE
        bne     loop1

        pla
        sta     fwdata_ptr
        creturn 2:fwdata_ptr

        end

