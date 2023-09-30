        .export     dispatch

        .import     _sp_dispatch
        .import     popa

; int8_t dispatch(uint8_t cmd, void *cmdlist)
;
; returns any error code from the smart port dispatch function
.proc dispatch
        sta     dispatch_data+1         ; cmdlist low
        stx     dispatch_data+2         ; cmdlist high

        jsr     popa                    ; cmd
        sta     dispatch_data

        jsr     do_dispatch
dispatch_data:
        .byte   $00             ; command
        .byte   $00             ; cmdlist low
        .byte   $00             ; cmdlist high

        rts

do_dispatch:
        jmp     (_sp_dispatch)

.endproc
