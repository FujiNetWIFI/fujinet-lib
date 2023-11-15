        .export     _fn_io_read_directory_block
        .import     fn_io_copy_cmd_data, popa, _fn_io_do_bus

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

; void *fn_io_read_directory_block(uint8_t maxlen, uint8_t pages, uint8_t extended_mode, void *buffer)
;
; pages is number of 256 blocks to request
; trashes tmp7-tmp10
.proc _fn_io_read_directory_block
        axinto  tmp7    ; 7/8 = buffer location

        ; setup DCB basic data
        setax   #t_io_read_directory_block
        jsr     fn_io_copy_cmd_data
        mwa     tmp7, IO_DCB::dbuflo

        popa    tmp10   ; extended mode, 1 = on, 0 = off
        
        jsr     popa    ; pages
        sta     IO_DCB::dbythi    ; pages to expect back 

        sec
        sbc     #$01    ; force into 0-7 range (from 1-8 from caller) to fit into 3 bits
        ora     #$C0    ; force bits 7&8 to mark this as block mode

        ; A = (PAGES | 0xC0) + if extended ? 0x20 : 0
        ldx     tmp10
        beq     :+      ; not extended

        ; Add extended mode flag
        ora     #$20    ; set the extended mode flag        

:       sta     IO_DCB::daux2   ; pages | 0xC0 | 0x20 if extended dir info requested

        jsr     popa            ; maxlen
        sta     IO_DCB::daux1   ; save in aux1

        jsr     _fn_io_do_bus
        setax   tmp7
        rts

.endproc

.rodata
t_io_read_directory_block:
        .byte $f6, $40, $00, $ff, $ff, $ff
