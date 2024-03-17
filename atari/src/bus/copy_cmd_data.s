        .export         copy_network_cmd_data
        .export         copy_fuji_cmd_data

        .include        "device.inc"
        .include        "macros.inc"
        .include        "zp.inc"

; INTERNAL COPY ROUTINE
; void copy_network_cmd_data(void *cmd_table)
;
; Sets DCB data from given table address
; Trashes tmp9/10 as only ZP location
copy_network_cmd_data:
        ; the table of dcb bytes to insert
        axinto  tmp9

        mva     #$71, IO_DCB::ddevic
        bne     common

copy_fuji_cmd_data:
        ; the table of dcb bytes to insert
        axinto  tmp9

        ; first 2 bytes always $70, $01, so we can do those manually. saves table space, and loops
        mva     #$70, IO_DCB::ddevic
        mva     #$01, IO_DCB::dunit

common:
        ; these 2 are always set by the command, no use having them in the table
        mva     #$00, IO_DCB::dbuflo
        sta     IO_DCB::dbufhi

        ; almost all devices use $0f
        mva     #$0f, IO_DCB::dtimlo

        ; copy bytes of table into DCB
        ldy     #5      ; 6 bytes to copy
l1:
        ldx     dcb_offsets, y
        mva     {(tmp9), y}, {IO_DCB::ddevic, x}
        dey
        bpl     l1

        rts


.rodata
; which DCB entries to write to, indexed from DDEVIC
; DCOMND, DSTATS, DBYTLO, DBYTHI, DAUX1, DAUX2
dcb_offsets: .byte 2, 3, 8, 9, 10, 11
