        .export     _fuji_copy_file

        .import     _bus
        .import     _fuji_success
        .import     copy_fuji_cmd_data
        .import     popa

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"
 
; bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
;
; slots are 0 based from caller, but FN needs them 1 based
; uses tmp7-10
.proc _fuji_copy_file
        axinto  tmp7    ; copyspec write location

        setax   #t_fuji_copy_file
        jsr     copy_fuji_cmd_data

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
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_copy_file:
        .byte $d8, $80, $00, $01, $ff, $ff
