                case    on              required for C compatibility
		mcopy    13/ainclude/m16.cc
		mcopy    13/orcainclude/m16.orca

; int8_t sp_dispatch(uint8_t cmd)
;
; returns any error code from the smart port _sp_dispatch function
sp_dispatch     start
                rtl                     TESTING
                sta     dispatch_data
                lda     #<sp_cmdlist
                sta     dispatch_data+1
                lda     #>sp_cmdlist
                sta     dispatch_data+2
        
; the SP dispatch alters the return address by 3 bytes to skip the data below.
; it returs with any error codes.
                dc      h'20'           ; JSR - making this a byte so we can get exact location of address being called
sp_dispatch_address entry               ; overwritten in sp_init to correct address
                dc      i2'0'

dispatch_data   dc      i1'0'           ; command
                dc      i1'0'           ; cmdlist low
                dc      i1'0'           ; cmdlist high

; A is error code. X,Y are the count of bytes
                sta     sp_error
                stx     sp_count
                sty     sp_count+1

; convert to a return value
                ldx     #$00
                lda     sp_error
                rtl

                end

