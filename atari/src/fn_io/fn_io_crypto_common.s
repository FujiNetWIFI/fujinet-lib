        .export     io_common

        .import     _fn_io_do_bus
        .import     _fn_io_error
        .import     fn_io_copy_cmd_data
        .import     popax

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "fn_data.inc"

.proc io_common
        jsr     fn_io_copy_cmd_data

        setax   tmp7
        sta     IO_DCB::dbytlo
        stx     IO_DCB::dbythi
        sta     IO_DCB::daux1
        stx     IO_DCB::daux2
        mva     #$03, IO_DCB::dtimlo

        jsr     popax           ; read the buffer location
        sta     IO_DCB::dbuflo
        stx     IO_DCB::dbufhi

        jsr     _fn_io_do_bus
        jmp     _fn_io_error
.endproc
