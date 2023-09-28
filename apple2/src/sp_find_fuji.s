        .export     _sp_find_fuji

        .import     spn_fn_d0
        .import     _sp_find_device

        .include    "macros.inc"
        .include    "zp.inc"

.proc _sp_find_fuji
        ; look for spn_fn_d0 in devices
        setax   #spn_fn_d0
        jmp     _sp_find_device
.endproc
