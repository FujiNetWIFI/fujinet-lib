        .export     _fuji_read_directory_block

        .import     _bus
        .import     _copy_fuji_cmd_data
        .import     _fuji_success

        .import     popa

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; void *fuji_read_directory_block(uint8_t ram_pages, uint8_t group_size, void *buffer)
;
; group_size is the number of files/dirs per PageGroup to allow client to cache efficiently
; ram_pages is number of 256 blocks to request.
; trashes tmp7-tmp10
.proc _fuji_read_directory_block
        axinto  tmp7    ; 7/8 = tmp buffer location to receive data

        ; setup DCB basic data
        setax   #t_fuji_read_directory_block
        jsr     _copy_fuji_cmd_data
        mwa     tmp7, IO_DCB::dbuflo

        jsr     popa            ; group_size (i.e. files per page group), valid numbers $00-$3F (0-63), although 0 would be pretty bad
        ora     #$C0            ; mark this as block read
        sta     IO_DCB::daux2

        jsr     popa            ; ram_pages count (number of 256 byte pages to use)
        sta     IO_DCB::dbythi  ; pages to expect back 
        sta     IO_DCB::daux1   ; also set in aux1 for firmware to use as buffer size

        jsr     _bus
        jmp     _fuji_success

.endproc

.rodata
t_fuji_read_directory_block:
        .byte $f6, $40, $00, $ff, $ff, $ff
