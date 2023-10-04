        .export     _network_json_query

        .import     _fn_network_bw
        .import     _fn_network_conn
        .import     _fn_network_error
        .import     _network_ioctl
        .import     _network_read
        .import     _network_status
        .import     popax
        .import     pusha
        .import     pushax
        .import     return0

        .include    "atari.inc"
        .include    "fujinet-network.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_json_query(char *devicespec, char *query, char *s);
;
_network_json_query:
        axinto  ptr1            ; save target string location
        popax   ptr2            ; save the input query location
        popax   ptr3            ; device spec

        ; call IOCTL with cmd Q
        pusha   #'Q'            ; cmd:  Query
        pusha   #$0c            ; aux1: read/write. could be a param?
        pusha   #$00            ; aux2: no translation
        pushax  ptr3            ; devicespec
        pushax  #$80            ; dstats: sending mode. this is varargs, so must be WORD
        pushax  ptr2            ; dbuf: query string
        setax   #$100           ; dbyt: 256 chars
        jsr     _network_ioctl  ; call ioctl
        bne     error

        ; perform a status to get the data length
        pushax  ptr3                    ; devicespec
        pushax  #_fn_network_bw         ; bytes waiting location
        pushax  #_fn_network_conn       ; connection status
        setax   #_fn_network_error      ; network error
        jsr     _network_status

        ; check for errors
        bne     error

        ; get the length of the read from DVSTAT
        mwa     DVSTAT, ptr2    ; reuse ptr2
        ora     DVSTAT          ; check for 0 length. A is currently DVSTAT+1, "or" with DVSTAT tells us if length is 0
        beq     no_data

        ; call network_read
        pushax  ptr3            ; devicespec
        pushax  ptr1            ; target string location
        setax   ptr2            ; length
        jsr     _network_read

        ; check for errors
        bne     error

        ; move string pointer on by len so we can nul terminate it
        adw     ptr1, ptr2      ; ptr1 += len

no_data:
        jsr     add_nul
        jmp     return0         ; FN_ERR_OK

error:
        sta     tmp1            ; save error code
        jsr     add_nul
        lda     tmp1
        ; error already gone through standardisation, so just return
        ; The caller should check error codes, and ignore any changes to string that may have happened
        rts

add_nul:
        ldy     #$00
        tya
        sta     (ptr1), y       ; add nul
        rts

; .endproc