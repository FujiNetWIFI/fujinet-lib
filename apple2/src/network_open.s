        .export     _network_open

        .import     _sp_init

; uint8_t network_open(char* devicespec, uint8_t trans)
.proc _network_open
        jsr     _sp_init

        

        rts
.endproc
