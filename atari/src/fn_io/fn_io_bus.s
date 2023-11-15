        .export _fn_io_bus

        .import fn_io_copy_cmd_data, _fn_io_do_bus

; void _fn_io_bus(void *dcb_table)
;
; Sets CMD data from passed in cmd_table and calls BUS
.proc _fn_io_bus
        jsr     fn_io_copy_cmd_data
        jmp     _fn_io_do_bus
.endproc
