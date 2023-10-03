        .export     _network_json_parse

        .import     _network_ioctl
        .import     popax
        .import     pusha
        .import     pushax

        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t network_json_parse(char *devicespec, uint8_t mode);
;
.proc _network_json_parse
        sta     tmp1            ; mode
        popax   ptr1            ; devicespec

        ; call IOCTL with cmd P
        pusha   #'P'            ; cmd:  Parse
        pusha   tmp1            ; aux1: read/write. could be a param?
        pusha   #$00            ; aux2: no translation
        pushax  ptr1            ; devicespec
        pushax  #$00            ; dstats: none. this is varargs, so must be WORD
        pushax  #$00            ; dbuf: 0
        setax   #$00            ; dbyt: 0
        jsr     _network_ioctl  ; call ioctl

        ; just return the error
        rts
.endproc