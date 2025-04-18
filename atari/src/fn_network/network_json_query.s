        .export     _network_json_query

        .import     _fn_bytes_read
        .import     _fn_device_error
        .import     _fn_network_bw
        .import     _fn_network_conn
        .import     _fn_network_error
        .import     _network_ioctl
        .import     _network_read
        .import     _network_status_unit
        .import     _network_unit
        .import     _sio_read
        .import     fn_open_mode_table
        .import     negax
        .import     popax
        .import     pusha
        .import     pushax
        .import     return0

        .include    "atari.inc"
        .include    "fujinet-network.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; int16_t network_json_query(const char *devicespec, const char *query, char *s);
;
; TODO: how do we deal with very large json results? Maybe interface with network_read, which can handle them.
; Or does sio_read work with any max size?

.proc _network_json_query
        axinto  tmp6            ; save target string location

        ldy     #$00
        sty     _fn_device_error

        popax   tmp3            ; save the input query location
        popax   ptr4            ; device spec
        jsr     _network_unit   ; get the unit so we can find the mode
        sta     tmp5            ; save unit
        tax                     ; convert to our table index for mode/trans values

        ; call IOCTL with cmd Q
        pusha   #'Q'            ; cmd:  Query
        lda     fn_open_mode_table-1, x
        jsr     pusha           ; aux1: mode for this connection from open - UNUSED IN Q COMMAND, setting it anyway
        lda     #$00
        jsr     pusha           ; aux2: I18N translation - NOT SET HERE. use ioctl 0xFB for this instead
        pushax  ptr4            ; devicespec
        pushax  #$80            ; dstats: sending mode. this is varargs, so must be WORD
        pushax  tmp3            ; dbuf: query string
        pushax  #$100           ; dbyt: 256 chars
        ldy     #$0b            ; varargs size
        jsr     _network_ioctl  ; call ioctl
        bne     error

        ; perform a status to get the data length
        pusha   tmp5                    ; unit
        pushax  #_fn_network_bw         ; bytes waiting location
        pushax  #_fn_network_conn       ; connection status
        setax   #_fn_network_error      ; network error
        jsr     _network_status_unit

        ; check for errors
        bne     error

        ; get the length of the read from DVSTAT
        lda     DVSTAT
        ora     DVSTAT+1        ; check for 0 length. A is currently DVSTAT+1, "or" with DVSTAT tells us if length is 0
        beq     no_data

        ; read data, this sets _fn_bytes_read
        pusha   tmp5            ; unit
        pushax  tmp6            ; buffer
        setax   DVSTAT          ; length
        jsr     _sio_read
        bne     error

        ; reduce bytes read by 1, as there's always an 0x9b line ending we don't require
        sbw1    _fn_bytes_read, #$01

        ; move string pointer on by len-1 (for atari) so we can nul terminate it (will overwrite the 0x9b)
        adw     tmp6, _fn_bytes_read

no_data:
        jsr     add_nul
        ; return the byte count
        ldx     _fn_bytes_read+1
        lda     _fn_bytes_read
        rts

error:
        sta     tmp1            ; save error code
        jsr     add_nul         ; sets the string to empty
        ; make the error negative
        ldx     #$00
        lda     tmp1
        jmp     negax

add_nul:
        ldy     #$00
        tya
        sta     (tmp6), y       ; add nul
        rts

.endproc

