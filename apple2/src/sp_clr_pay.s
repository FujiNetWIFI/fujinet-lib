        .export     _sp_clr_pay

        .import     _bzero
        .import     _sp_payload
        .import     pushax

        .include    "macros.inc"

; void 

.proc _sp_clr_pay
        pushax  #_sp_payload
        setax   #$400
        jmp     _bzero
.endproc