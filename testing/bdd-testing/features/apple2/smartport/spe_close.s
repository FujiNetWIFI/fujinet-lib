        .export     close_fnd0
        .export     close_printer
        .export     close_network
        .export     close_clock
        .export     close_modem
        .export     close_cpm

        .import     return0

        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t close_fnd0(void *cmdList)
;
; Handle fujinet sp_close calls
.proc close_fnd0
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t close_printer(void *cmdList)
;
; Handle printer sp_close calls
.proc close_printer
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t close_network(void *cmdList)
;
; Handle network sp_close calls
.proc close_network
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t close_clock(void *cmdList)
;
; Handle clock sp_close calls
.proc close_clock
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t close_modem(void *cmdList)
;
; Handle modem sp_close calls
.proc close_modem
        axinto  ptr1

        jmp     return0
.endproc

; uint8_t close_cpm(void *cmdList)
;
; Handle cpm sp_close calls
.proc close_cpm
        axinto  ptr1

        jmp     return0
.endproc

