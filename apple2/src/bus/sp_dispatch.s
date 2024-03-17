        .export     _sp_dispatch

        .import     _sp_dispatch_fn
        .import     popa

; int8_t sp_dispatch(uint8_t cmd, void *cmdlist)
;
; returns any error code from the smart port _sp_dispatch function
.proc _sp_dispatch
        sta     dispatch_data+1         ; cmdlist low
        stx     dispatch_data+2         ; cmdlist high

        jsr     popa                    ; cmd
        sta     dispatch_data

        jsr     do_jmp
dispatch_data:
        .byte   $00             ; command
        .byte   $00             ; cmdlist low
        .byte   $00             ; cmdlist high

        rts

do_jmp:
        jmp     (_sp_dispatch_fn)

.endproc
