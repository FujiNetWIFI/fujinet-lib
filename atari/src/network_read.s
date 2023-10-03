        .export     _network_read

        .import     network_rw

        .include    "zp.inc"
        .include    "macros.inc"

; uint8_t network_read(char* devicespec, uint8_t *buf, uint16_t len)
_network_read:
        axinto  tmp7                    ; len
        setax   #t_network_read
        jmp     network_rw

.rodata
t_network_read:
        .byte 'R', $40, $ff, $ff, $ff, $ff
