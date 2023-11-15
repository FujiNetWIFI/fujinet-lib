        .export     _sp_clr_payload

        .import     _bzero
        .import     _sp_payload
        .import     pushax

        .include    "macros.inc"

; void sp_clr_payload();

.proc _sp_clr_payload
        pushax  #_sp_payload
        setax   #$400
        jmp     _bzero
.endproc