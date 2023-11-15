        .export     _main

        .import     _sp_find_network
        .import     _sp_init
        .import     spe_num_devices

        .include    "macros.inc"

.proc _main
        jsr     _sp_init
        ; make the emulator return count of 0 devices
        lda     #$00
        sta     spe_num_devices
        jmp     _sp_find_network
.endproc

