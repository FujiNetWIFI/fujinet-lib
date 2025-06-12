        .export     __STARTUP__ : absolute = 1
        .export     start
        .export     _halt

        .import     _main
        .import     __HIMEM__

        .include    "zeropage.inc"


.segment "STARTUP"
start:
        ; set INTERRUPT/NMI vectors to a _halt address, which issues a STP
        ; this will cause any "BRK" in the application to call "_halt", and thus stop the emulator
        lda     #<_halt
        sta     $FFFE   ; INTERRUPT
        sta     $FFFA   ; NMI
        lda     #>_halt
        sta     $FFFF   ; INTERRUPT
        sta     $FFFB   ; NMI

        ; setup software stack pointer
        lda     #<__HIMEM__
        sta     sp
        lda     #>__HIMEM__
        sta     sp+1

        ; setup 6502 stack pointer
        ldx     #$ff
        txs

        ; clean up a bit for start of application
        inx             ; X = 0
        txa
        tay
        clc

        ; call main
        jmp     _main

_halt:
        .byte   $db         ; STP in 65c02 emulator

.segment "ONCE"
        rts

.segment "INIT"
        rts

; ensures V_RESET is defined, otherwise __V_RESET_LOAD__ isn't defined.
.segment "V_RESET"
        rts