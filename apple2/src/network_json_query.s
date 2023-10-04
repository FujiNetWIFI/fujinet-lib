        .export     _network_json_query

        .import     _bad_unit
        .import     _fn_error
        .import     _memcpy
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
        axinto  _sp_payload     ; sp_payload[0..1]

        ; remove the devicespec parameter from stack, it isn't used yet by apple, the sp_network unit already found
        jsr     incsp2

        ; copy query string into payload
        pushax  #_sp_payload+2  ; dest
        pushax  ptr1            ; src
        setax   _sp_payload     ; length in payload[0..1]
        jsr     _memcpy         ; trashes ptr1-3, but we don't need ptr1 anymore

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
        mwa     _sp_payload, tmp1       ; keep length in tmp1/2

        ; check length > 0, a is currently _sp_payload+1, high byte. "ora" with lo byte in tmp1
        ora     tmp1
        bne     not_empty

        ; there was 0 data to read for this query
        jsr     add_nul
        jmp     return0

;----------------------------------------------------------------------------------------
; put in the middle so all code can reach it
error:
        sta     tmp1            ; store error while we deal with nulling string
        jsr     add_nul
        lda     tmp1
        jmp     _fn_error

add_nul:
        ; tmp5/6 points to current target string, send back an empty string
        ldy     #$00
        tya
        sta     (tmp5), y
        rts
;----------------------------------------------------------------------------------------

        ; ----------------------------------------------------------------------------------
        ; read
not_empty:
        pusha   _sp_network
        ; length of waiting bytes to read in tmp1/2
        setax   tmp1
        jsr     _sp_read
        bne     error

        ; add a terminal nul to end of data, then use strncpy to copy into target.
        mwa     #_sp_payload+2, ptr1
        adw     ptr1, tmp1      ; ptr1 += len
        lda     #$00
        tay
        sta     (ptr1), y

        ; copy to destination
        pushax  tmp5            ; dst
        pushax  #_sp_payload+2   ; src
        ; increment the length by 1 to guarantee a nul is copied
        lda     tmp1
        ldx     tmp2
        clc
        adc     #$01
        bcc     :+
        inx
:       jsr     _strncpy
        jmp     return0         ; FN_ERR_OK

.endproc
