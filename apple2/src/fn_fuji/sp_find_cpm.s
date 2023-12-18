        .export     _sp_find_cpm

        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_cpm
        ; look for sp_cpm in devices
        setax   #sp_cpm
        jmp     _sp_find_device
.endproc

.data
sp_cpm:         .byte "CPM", 0
