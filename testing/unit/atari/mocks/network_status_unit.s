.export _network_status_unit
.import incsp5

; ##########################################################
.code

_network_status_unit:
    ; remove the parameters from software stack
    jsr     incsp5

    ; mark that we were called by incrementing the count
    inc     nw_status_was_called
    rts

; ##########################################################
.bss

.export nw_status_was_called
nw_status_was_called: .byte 0
