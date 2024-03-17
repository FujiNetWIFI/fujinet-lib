        .export     io_common

        .import     _bus
        .import     _fuji_error
        .import     copy_fuji_cmd_data
        .import     popax

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "device.inc"

.proc io_common
        jsr     copy_fuji_cmd_data

        setax   tmp7
        sta     IO_DCB::dbytlo
        stx     IO_DCB::dbythi
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2
        mva     #$03, IO_DCB::dtimlo

        jsr     popax           ; read the buffer location
        sta     IO_DCB::dbuflo
        stx     IO_DCB::dbufhi

        jsr     _bus
        jmp     _fuji_error
.endproc
