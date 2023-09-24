        .export     _io_status

        .import     _network_status_unit

        .include    "atari.inc"
        .include    "device.inc"
        .include    "zp.inc"

; uint8_t io_status(uint8_t unit)
.proc _io_status
        sta     tmp8                ; unit

        ldx     #$00                ; high byte of return
        lda     IO_DCB::dstats
        cmp     #DERROR
        beq     @extended
        rts

@extended:
        lda     tmp8
        jmp     _network_status_unit
.endproc