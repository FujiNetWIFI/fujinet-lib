        .export         _fn_fuji_set_hsio_index

        .import         copy_fuji_cmd_data
        .import         _bus
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; void fn_fuji_set_hsio_index(bool save, uint8_t index);
;
.proc _fn_fuji_set_hsio_index
        sta     tmp7                    ; HSIO index to set
        setax   #t_fn_fuji_set_hsio_index
        jsr     copy_fuji_cmd_data

        mva     tmp7, IO_DCB::daux1
        jsr     popa                    ; should we save? 0 = no, 1 = yes
        sta     IO_DCB::daux2

        jmp     _bus
.endproc

.rodata
t_fn_fuji_set_hsio_index:
        .byte $e3, 0, 0, 0, $ff, $ff
