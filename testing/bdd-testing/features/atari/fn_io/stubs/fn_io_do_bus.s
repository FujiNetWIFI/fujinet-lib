                .export   _fn_io_do_bus

                .include  "device.inc"

; required implementation of _fn_io_do_bus
.proc _fn_io_do_bus
        jmp     BUS
.endproc