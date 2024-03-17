        .export         _fn_fuji_hash_output

        .import         _bus
        .import         _fn_fuji_error
        .import         copy_fuji_cmd_data
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; uint8_t fn_fuji_hash_output(uint8_t output_type, char *s, uint16_t len);
;
.proc _fn_fuji_hash_output
        axinto  tmp7                    ; len pointer

        setax   #t_fn_fuji_hash_output
        jsr     copy_fuji_cmd_data

        setax   tmp7
        sta     IO_DCB::dbytlo
        stx     IO_DCB::dbythi
        mva     #$03, IO_DCB::dtimlo
        jsr     popax                   ; buffer address (s)
        sta     IO_DCB::dbuflo
        stx     IO_DCB::dbufhi
        jsr     popa                    ; output type (0 = bin, 1 = hex)
        sta     IO_DCB::daux1

        jsr     _bus
        jmp     _fn_fuji_error
.endproc

.rodata
t_fn_fuji_hash_output:
        .byte $c5, $40, $ff, $ff, 1, 0
