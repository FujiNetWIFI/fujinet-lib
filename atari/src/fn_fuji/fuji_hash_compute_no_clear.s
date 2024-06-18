        .export         _fuji_hash_compute_no_clear

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data
        .import         popa, popax

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"

; bool fuji_hash_compute_no_clear(uint8_t type);
;
; functionally same as fuji_hash_compute, but the FujiNet does not clear the stored data being hashed internally after computing.

.proc _fuji_hash_compute_no_clear
        sta     tmp7                    ; hash type, one of HashType. No validation done though

        setax   #t_fuji_hash_compute_no_clear
        jsr    _copy_fuji_cmd_data

        mva     tmp7, IO_DCB::daux1
        mva     #$03, IO_DCB::dtimlo
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata
t_fuji_hash_compute_no_clear:
        .byte $c3, $00, $00, $00, $ff, $00
