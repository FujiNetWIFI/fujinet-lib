        .export     _sp_find_fuji

        .import     sp_fn_d0
        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_fuji
        ; look for sp_fn_d0 in devices
        setax   #sp_fn_d0
        jmp     _sp_find_device
.endproc
