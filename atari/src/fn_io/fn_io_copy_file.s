        .export     _fn_io_copy_file
        .import     fn_io_copy_cmd_data, popa, _fn_io_do_bus
        .include    "zp.inc"
        .include    "macros.inc"
        .include    "fn_data.inc"
 
; void fn_io_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
;
; slots are 0 based from caller, but FN needs them 1 based
; uses tmp7-10
.proc _fn_io_copy_file
        axinto  tmp7    ; copyspec write location

        setax   #t_io_copy_file
        jsr     fn_io_copy_cmd_data

        ;  fujinet tracks 1-8, we do 0-7, so need to increment both values
        jsr     popa            ; dst slot -> daux2
        clc
        adc     #$01
        sta     IO_DCB::daux2

        jsr     popa            ; src slot -> daux1
        clc
        adc     #$01
        sta     IO_DCB::daux1

        mwa     tmp7,  IO_DCB::dbuflo
        mva     #$fe,  IO_DCB::dtimlo
        jmp     _fn_io_do_bus
.endproc

.rodata
t_io_copy_file:
        .byte $d8, $80, $00, $01, $ff, $ff
