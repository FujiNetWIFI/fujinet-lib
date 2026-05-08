        .export     __STARTUP__ : absolute = 1
        .export     start
        .export     end_setup

        .include    "zeropage.inc"

.segment "STARTUP"
start:
        ; set INTERRUPT/NMI vectors to a _halt address, which issues a STP
        ; this will cause any "BRK" in the application to call "_halt", and thus stop the emulator
        lda     #<halt
        sta     $FFFE   ; INTERRUPT
        sta     $FFFA   ; NMI
        lda     #>halt
        sta     $FFFF   ; INTERRUPT
        sta     $FFFB   ; NMI

        ; set software stack at EFFF
        lda     #$FF
        ldx     #$EF
        sta     c_sp
        stx     c_sp+1

        ; setup stack pointer to something sensible
        ldx     #$ff
        txs

        ; clean up a bit for start of application
        inx             ; X = 0
        txa
        tay
        clc

end_setup:

halt:
        .byte   $db         ; STP in 65c02 emulator

; this will cause the emulator to set the address "init" to the "start" address, so you can use "run init until CP = end_setup"
.segment "V_RESET"
        .word start
