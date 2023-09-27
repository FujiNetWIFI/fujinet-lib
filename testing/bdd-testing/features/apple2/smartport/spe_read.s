        .export     read_fnd0
        .export     read_printer
        .export     read_network
        .export     read_clock
        .export     read_modem
        .export     read_cpm

        .import     return0

        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t read_fnd0(void *cmdList)
;
; Handle fujinet sp_read calls, reacting to the parameters given in the cmdList for testing
.proc read_fnd0
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t read_printer(void *cmdList)
;
; Handle printer sp_read calls, reacting to the parameters given in the cmdList for testing
.proc read_printer
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t read_network(void *cmdList)
;
; Handle network sp_read calls, reacting to the parameters given in the cmdList for testing
.proc read_network
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t read_clock(void *cmdList)
;
; Handle clock sp_read calls, reacting to the parameters given in the cmdList for testing
.proc read_clock
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t read_modem(void *cmdList)
;
; Handle modem sp_read calls, reacting to the parameters given in the cmdList for testing
.proc read_modem
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t read_cpm(void *cmdList)
;
; Handle cpm sp_read calls, reacting to the parameters given in the cmdList for testing
.proc read_cpm
        axinto  ptr1

        jmp     return0
.endproc

