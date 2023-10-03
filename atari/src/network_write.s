        .export     _network_write

        .import     network_rw

        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_write(char* devicespec, uint8_t *buf, uint16_t len)
_network_write:
        axinto  tmp7                    ; len
        setax   #t_network_write
        jmp     network_rw

t_network_write:
        .byte 'W', $80, $ff, $ff, $ff, $ff
