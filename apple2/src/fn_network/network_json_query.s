        .export     _network_json_query

        .import     _bad_unit
        .import     _fn_device_error
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
        .import     popax
        .import     pusha
        .import     pushax
        .import     return0

        ; .import     _sp_clr_payload
        ; .import     _hd

        .include    "sp.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; int16_t network_json_query(char *devicespec, char *query, char *s);
;
.proc _network_json_query
        axinto  tmp5            ; save string output location

        ; jsr     _sp_clr_payload     ; calls bzero, so trashes p1/2/3

        ldy     #$00
        sty     _fn_device_error

        ; is there a network id?
        lda     _sp_network     ; get network id
        bne     :+

        ; remove 2 word args on stack, and return an error
        jsr     incsp4
        jsr     _bad_unit
        ; make A negative for an error code. X already 0.
        eor     #$ff
        clc
        adc     #$01
        rts


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

        ;; NOT REQUIRED
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

        ; there was 0 data to read for this query, return 0 length
        jsr     add_nul
        jmp     return0

;----------------------------------------------------------------------------------------
; put in the middle so all code can reach it
error:
        sta     ptr4            ; store error while we deal with nulling string
        jsr     add_nul
        lda     ptr4
        jsr     _fn_error
        ; make it negative
        eor     #$ff
        clc
        adc     #$01
        rts


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

        ; copy to destination
        pushax  tmp5            ; dst
        pushax  #_sp_payload    ; src, 0 based after a read
        setax   ptr4            ; len
        jsr     _memcpy         ; doesn't touch ptr4.

        ; ----------------------------------------------
        ; DEBUG hex dump the retruned string.
        ; pushax  tmp5
        ; pushax  ptr4

        ; pushax  tmp5
        ; setax   ptr4            ; length
        ; jsr     _hd

        ; popax   ptr4
        ; popax   tmp5
        ; ----------------------------------------------

        ; nul terminate the string
        adw     tmp5, ptr4      ; set tmp5 to end of string
        sbw1    tmp5, #$01

        ; ----------------------------------------------
        ; pushax  tmp5

        ; pushax  tmp5
        ; setax   #$08
        ; jsr     _hd

        ; popax   tmp5
        ; ----------------------------------------------

        ; check if last char is 9b/0d/0a, if it is not, move on 1 char before nul terminating
        ldy     #$00
        lda     (tmp5), y
        cmp     #$9b
        beq     @skip_add
        cmp     #$0d
        beq     @skip_add
        cmp     #$0a
        beq     @skip_add

        adw1    tmp5, #$01

@skip_add:
        tya
        sta     (tmp5), y

        ; return the length held in ptr4 - does this need adjusting for 9b/0d/0a?
        setax   ptr4
        rts

.endproc
