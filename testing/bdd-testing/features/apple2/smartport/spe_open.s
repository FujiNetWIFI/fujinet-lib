        .export     open_fnd0
        .export     open_printer
        .export     open_network
        .export     open_clock
        .export     open_modem
        .export     open_cpm

        .import     return0

        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t open_fnd0(void *cmdList)
;
; Handle fujinet sp_open calls, reacting to the parameters given in the cmdList for testing
.proc open_fnd0
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t open_printer(void *cmdList)
;
; Handle printer sp_open calls, reacting to the parameters given in the cmdList for testing
.proc open_printer
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t open_network(void *cmdList)
;
; Handle network sp_open calls, reacting to the parameters given in the cmdList for testing
.proc open_network
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t open_clock(void *cmdList)
;
; Handle clock sp_open calls, reacting to the parameters given in the cmdList for testing
.proc open_clock
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t open_modem(void *cmdList)
;
; Handle modem sp_open calls, reacting to the parameters given in the cmdList for testing
.proc open_modem
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t open_cpm(void *cmdList)
;
; Handle cpm sp_open calls, reacting to the parameters given in the cmdList for testing
.proc open_cpm
        axinto  ptr1

        jmp     return0
.endproc

