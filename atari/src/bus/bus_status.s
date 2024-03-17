        .export     _bus_status

        .import     _fn_device_error
        .import     _fn_error
        .import     _fn_network_bw
        .import     _fn_network_conn
        .import     _fn_network_error
        .import     _network_status_unit
        .import     pusha
        .import     pushax

        .include    "atari.inc"
        .include    "device.inc"
        .include    "macros.inc"
        .include    "zp.inc"

; uint8_t io_status(uint8_t unit)
;
; unit is only used when dstats is equal to DERROR (144) for extended information
.proc _bus_status
        sta     tmp8                    ; unit

        ldx     #$00                    ; high byte of return
        lda     IO_DCB::dstats
        sta     _fn_device_error        ; keep the raw status value
        cmp     #DERROR
        beq     @extended
        jmp     _fn_error

@extended:
        pusha   tmp8                    ; unit
        pushax  #_fn_network_bw         ; bytes waiting location
        pushax  #_fn_network_conn       ; connection status
        setax   #_fn_network_error      ; network error
        jmp     _network_status_unit
.endproc