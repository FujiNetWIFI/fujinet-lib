; uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);

                case    on              required for C compatibility
                mcopy   13/ainclude/m16.cc      csubroutine & creturn macros
                mcopy   13/orcainclude/m16.orca assembler macros (short, long etc.)

clock_get_time  start
                csubroutine (4:time_data,2:format),2
    
res             equ     1                

                jsl     sp_get_clock_id
                bne     got_id

; no clock found, return 1 as an error (FN_ERR_IO_ERROR)
error           lda     #1
                sta     res
                creturn 2:res

; call sp_status(uint8_t dest, uint8_t statcode)

; convert the time format to the appropriate device specific code.
; SIMPLE_BINARY     (0) -> 'T'
; PRODOS_BINARY     (1) -> 'P'
; APETIME_TZ_BINARY (2) -> 'A'
; APETIME_BINARY    (3) -> 'B'
; TZ_ISO_STRING     (4) -> 'S'
; UTC_ISO_STRING    (5) -> 'Z'

got_id          tay
                ldx     format
; ensure the value is valid
                cpx     #$06
                bcs     error
                lda     code_table,x
                and     #$00ff          mask high byte
                pha                     command
                phy                     clock device id
                jsl     sp_status
                bne     error

; results are in sp_payload, the clock data returned is small (4 to 26 bytes)
; sp_count holds the number of bytes to copy from sp_payload, will always contain at least 1 byte (null terminator)
                short   m
                ldy     #0
loop            lda     sp_payload,y
                sta     [time_data],y
                iny
                cpy     sp_count
                bne     loop
                long    m

; time read, return 0 (FN_ERR_OK)                
                stz     res
                creturn 2:res

code_table      dc      c'TPABSZ'

                end

