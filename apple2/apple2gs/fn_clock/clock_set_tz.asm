; uint8_t clock_set_tz(char *tz);

                case    on              required for C compatibility
                mcopy   13/ainclude/m16.cc      csubroutine & creturn macros
                mcopy   13/orcainclude/m16.orca assembler macros (short, long etc.)

clock_set_tz    start
                csubroutine (4:tz),2

res             equ     1                

                jsl     sp_get_clock_id
                bne     got_id

; no clock found, return 1 as an error (FN_ERR_IO_ERROR)
error           lda     #1
                sta     res
                creturn 2:res

got_id          pha                     store the destination device for call to sp_control

; copy timezone string into sp_payload, including the null terminator
                short   m
                ldy     #0
loop            lda     [tz],y
                sta     sp_payload+2,y
                beq     terminator
                iny
                bra     loop
terminator      long    m
                iny                     increment for the null terminator of the string
                sty     sp_payload      store the length for sp_control call

                pla
                ph2     #'T'            set 'T'imezone
                pha                     clock id
                jsl     sp_control
                bne     error

; tz set, return 0 (FN_ERR_OK)                
                stz     res
                creturn 2:res

                end

