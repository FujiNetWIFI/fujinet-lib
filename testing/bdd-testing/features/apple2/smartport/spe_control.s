        .export     control_fnd0
        .export     control_printer
        .export     control_network
        .export     control_clock
        .export     control_modem
        .export     control_cpm

        .import     return0

        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t control_fnd0(void *cmdList)
;
; Handle fujinet sp_control calls
.proc control_fnd0
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t control_printer(void *cmdList)
;
; Handle printer sp_control calls
.proc control_printer
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t control_network(void *cmdList)
;
; Handle network sp_control calls
.proc control_network
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t control_clock(void *cmdList)
;
; Handle clock sp_control calls
.proc control_clock
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t control_modem(void *cmdList)
;
; Handle modem sp_control calls
.proc control_modem
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t control_cpm(void *cmdList)
;
; Handle cpm sp_control calls
.proc control_cpm
        axinto  ptr1

        jmp     return0
.endproc

