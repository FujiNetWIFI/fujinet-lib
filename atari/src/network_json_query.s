        .export     _network_json_query
        .export     add_nul

        .import     _network_ioctl
        .import     _network_read
        .import     _network_status_unit
        .import     _network_unit
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
        setax   ptr3            ; devicespec
        jsr     _network_unit   ; get the unit. this allows us to call the optimised version of status without stack pointer hacks
        jsr     _network_status_unit

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