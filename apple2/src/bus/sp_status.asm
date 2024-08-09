                case    on              required for C compatibility
		mcopy   13/ainclude/m16.cc
		mcopy   13/orcainclude/m16.orca

; set A to the code to store in sp_cmdlist[4]
; the dest param is expected to be on s/w stack as 1 byte

sp_status start
        csubroutine (2:dest,2:statcode),4       4 bytes direct page space

        copy    apple2:src:include:sp.equ

fwdata_ptr      equ 1                   32-bit ptr

        ldx     sp_fwdata
        stx     fwdata_ptr
        stz     fwdata_ptr+2
        lda     #SP_STATUS_PARAM_COUNT
        ldy     #$400                   cmdlist offset
        sta     [fwdata_ptr],y
        lda     dest
        iny                             cmdlist offset + 1
        sta     [fwdata_ptr],y
        txa
        iny                             cmdlist offset + 2
        sta     [fwdata_ptr],y
        lda     statcode
        iny
        iny                             cmdlist offset + 4
        short   m
        sta     [fwdata_ptr],y
        long    m

        lda     #SP_CMD_STATUS
        jsl     sp_dispatch             return sp_dispatch(SP_CMD_STATUS);
        sta     fwdata_ptr

        creturn 2:fwdata_ptr

        end