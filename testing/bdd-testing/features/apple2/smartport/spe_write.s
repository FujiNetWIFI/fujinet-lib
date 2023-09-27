        .export     write_fnd0
        .export     write_printer
        .export     write_network
        .export     write_clock
        .export     write_modem
        .export     write_cpm

        .import     return0

        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t write_fnd0(void *cmdList)
;
; Handle fujinet sp_write calls
.proc write_fnd0
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t write_printer(void *cmdList)
;
; Handle printer sp_write calls
.proc write_printer
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t write_network(void *cmdList)
;
; Handle network sp_write calls
.proc write_network
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t write_clock(void *cmdList)
;
; Handle clock sp_write calls
.proc write_clock
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t write_modem(void *cmdList)
;
; Handle modem sp_write calls
.proc write_modem
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t write_cpm(void *cmdList)
;
; Handle cpm sp_write calls
.proc write_cpm
        axinto  ptr1

        jmp     return0
.endproc

