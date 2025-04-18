        .export     _network_json_parse

        .import     _fn_device_error
        .import     _network_ioctl
        .import     _network_unit
        .import     fn_open_mode_table
        .import     popax
        .import     pusha
        .import     pushax

        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_json_parse(const char *devicespec);
;
.proc _network_json_parse
        axinto  ptr1            ; devicespec

        ldy     #$00
        sty     _fn_device_error

        jsr     _network_unit   ; this is lookup index for the mode
        tax
        ; mode for this unit (which is 1 based)
        lda     fn_open_mode_table-1, x
        sta     tmp1

        ; set channel mode to json
        pusha   #$FC            ; cmd:  Set channel mode
        pusha   tmp1            ; aux1: open-mode - NOT USED BY PIO
        pusha   #$01            ; aux2: CHANNELMODE_JSON
        pushax  ptr1            ; devicespec
        pushax  #$00            ; dstats: none. this is varargs, so must be WORD
        pushax  #$00            ; dbuf: 0
        pushax  #$00            ; dbyt: 0
        ldy     #$0b            ; varargs size
        jsr     _network_ioctl  ; call ioctl
        bne     error

        ; call IOCTL with cmd P
        pusha   #'P'            ; cmd:  Parse
        pusha   tmp1            ; aux1: read/write
        pusha   #$00            ; aux2: no translation
        pushax  ptr1            ; devicespec
        pushax  #$00            ; dstats: none. this is varargs, so must be WORD
        pushax  #$00            ; dbuf: 0
        pushax  #$00            ; dbyt: 0
        ldy     #$0b            ; varargs size
        jsr     _network_ioctl  ; call ioctl

error:
        ; just return the error
        rts
.endproc