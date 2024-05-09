        .export         _fuji_set_hsio_index

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         popa

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_set_hsio_index(bool save, uint8_t index);
;
.proc _fuji_set_hsio_index
        sta     tmp7                    ; HSIO index to set
        setax   #t_fuji_set_hsio_index
        jsr     _copy_fuji_cmd_data

        mva     tmp7, IO_DCB::daux1
        jsr     popa                    ; should we save? 0 = no, 1 = yes
        sta     IO_DCB::daux2

        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_set_hsio_index:
        .byte $e3, 0, 0, 0, $ff, $ff
