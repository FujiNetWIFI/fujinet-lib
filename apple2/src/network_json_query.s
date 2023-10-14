        .export     _network_json_query

        .import     _bad_unit
        .import     _fn_device_error
        .import     _fn_error
        .import     _memcpy
        .import     _sp_clr_payload
        .import     _sp_control
        .import     _sp_network
        .import     _sp_payload
        .import     _sp_read
        .import     _sp_status
        .import     _strlen
        .import     _strncpy
        .import     incsp2
        .import     incsp4
        .import     popa
        .import     popax
        .import     pusha
        .import     pushax
        .import     return0

        .include    "sp.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_json_query(char *devicespec, char *query, char *s);
;
.proc _network_json_query
        axinto  tmp5            ; save string output location

        jsr     _sp_clr_payload     ; calls bzero, so trashes p1/2/3

        ldy     #$00
        sty     _fn_device_error

        ; is there a network id?
        lda     _sp_network     ; get network id
        bne     :+

        ; remove 2 word args on stack, and return an error
        jsr     incsp4
        jmp     _bad_unit

        ; ----------------------------------------------------------------------------------
        ; Query
        ; sp_payload[0..1]     = length of query
        ; sp_payload[2..2+len] = query string
:       popax   ptr1            ; query string location
        jsr     _strlen
        ; ADD 1 for NUL char we need to send
        clc
        adc     #$01
        bcc     :+
        inx
:       axinto  _sp_payload     ; sp_payload[0..1]

        ; remove the devicespec parameter from stack, it isn't used yet by apple, the sp_network unit already found
        jsr     incsp2

        ; copy query string into payload
        pushax  #_sp_payload+2  ; dest
        pushax  ptr1            ; src
        setax   _sp_payload     ; length from payload[0..1]
        jsr     _strncpy        ; trashes ptr1-2, but we don't need ptr1 anymore, returns dest

        ;; NOT REQUIRED - we have zero'd whole of sp_payload previous to the strncpy        
        ; ; add a 0 to end of query string
        ; axinto  ptr1            ; move sp_payload+2 location into ptr1
        ; adw     ptr1, _sp_payload       ; increment ptr1 by length of query string
        ; ldy     #$00
        ; tya
        ; sta     (ptr1), y

        pusha   _sp_network
        lda     #'Q'
        jsr     _sp_control
        bne     error

        ; ----------------------------------------------------------------------------------
        ; Status - fetch count of bytes to read
        pusha   _sp_network
        lda     #'S'
        jsr     _sp_status
        bne     error
        mwa     _sp_payload, ptr4       ; keep length (currently in sp_payload[0,1]) in ptr4

        ; check length > 0, a is currently _sp_payload+1, high byte. "ora" with lo byte in ptr4
        ora     ptr4
        bne     not_empty

        ; there was 0 data to read for this query
        jsr     add_nul
        jmp     return0

;----------------------------------------------------------------------------------------
; put in the middle so all code can reach it
error:
        sta     ptr4            ; store error while we deal with nulling string
        jsr     add_nul
        lda     ptr4
        jmp     _fn_error

add_nul:
        ; tmp5 points to current target string, send back an empty string
        ldy     #$00
        tya
        sta     (tmp5), y
        rts
;----------------------------------------------------------------------------------------

        ; ----------------------------------------------------------------------------------
        ; read
not_empty:
        pusha   _sp_network
        ; length of waiting bytes to read in ptr4
        setax   ptr4
        jsr     _sp_read
        bne     error

        ; ; add nul - not required, we use memcpy instead to do exact count, then add nul afterwards
        ; mwa     #_sp_payload+2, ptr1
        ; adw     ptr1, ptr4      ; ptr1 += len
        ; ldy     #$00
        ; tya
        ; sta     (ptr1), y

        ; copy to destination
        pushax  tmp5            ; dst
        pushax  #_sp_payload+2  ; src
        setax   ptr4            ; len
        jsr     _memcpy         ; doesn't touch ptr4.

        ; nul terminate the string
        adw     tmp5, ptr4      ; set tmp5 to end of string
        ldy     #$00
        tya
        sta     (tmp5), y

        jmp     return0         ; FN_ERR_OK
.endproc
