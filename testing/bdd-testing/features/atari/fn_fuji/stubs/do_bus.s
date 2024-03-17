                .export   _bus

                .include  "device.inc"

; required implementation of _bus
.proc _bus
        jmp     BUS
.endproc