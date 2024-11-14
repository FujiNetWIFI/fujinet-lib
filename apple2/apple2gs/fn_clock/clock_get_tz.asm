; uint8_t clock_get_tz(uint8_t* tz);

                case    on              required for C compatibility
                mcopy   13/ainclude/m16.cc      csubroutine & creturn macros
                mcopy   13/orcainclude/m16.orca assembler macros (short, long etc.)

clock_get_tz    start
                csubroutine (4:tz),2

res             equ     1                

                jsl     sp_get_clock_id
                bne     got_id

; no clock found, return 1 as an error (FN_ERR_IO_ERROR)
error           lda     #1
                sta     res
                creturn 2:res

; sp_status(clock_id, 'G')
got_id          ph2     #'G'            set 'G'et timezone
                pha                     clock id
                jsl     sp_status
                bne     error

; results are in sp_payload, the clock data returned is small (4 to 26 bytes)
; sp_count holds the number of bytes to copy from sp_payload, will always contain at least 1 byte (null terminator)
                short   m
                ldy     #0
loop            lda     sp_payload,y
                sta     [tz],y
                iny
                cpy     sp_count
                bne     loop
                long    m

; tz read, return 0 (FN_ERR_OK)                
                stz     res
                creturn 2:res

                end

