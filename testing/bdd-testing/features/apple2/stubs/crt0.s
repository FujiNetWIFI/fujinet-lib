; apple2 test crt0.s file

        .export     _exit
        .export     _init
        .export     __STARTUP__ : absolute = 1

        .import     __HIMEM__
        .import     _main
        .import     _setup_smartport
        .import     initlib
        .import     return0

        .include   "macros.inc"
        .include   "zp.inc"

        .segment   "STARTUP"

_init:
        ldx     #$ff
        txs

        ; This is important to ensure any stack usage (pusha, etc) get somewhere to use.
        ; Remember, stack works down, so set sp to the top of the area to use.
        mwa     #__HIMEM__, sp

        ; Set a default callback for SmartPort emulator that just returns OK (0).
        ; Tests can change this vector for their own use later. setup only needs to run once to create
        ; the dispatcher, and a fake smartport on slot 1.
        setax   #smartport_cb
        jsr     _setup_smartport

        ; start the application
        jsr     _main

_exit:  rts

; default callback for tests is to return OK (0)
smartport_cb:
        jmp     return0

        .data
x_data: .byte 0

        .rodata
x_ro:   .byte 0
