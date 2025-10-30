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

; uint8_t bus_status(uint8_t unit)
;
; unit is only used when dstats is equal to DERROR (144) for extended information
.proc _bus_status
        sta     tmp8                    ; unit

        ldx     #$00                    ; high byte of return
        lda     IO_DCB::dstats
        cmp     #DERROR
        beq     @extended
@exit:
        jmp     _fn_error

@extended:
        pusha   tmp8                    ; unit
        pushax  #_fn_network_bw         ; bytes waiting location
        pushax  #_fn_network_conn       ; connection status
        setax   #_fn_network_error      ; network error
        jsr     _network_status_unit    ; fill in the status bytes into memory locations given

        ; we must restore the ERROR code from the original call, not leave ourselves with the network_status result code, which is "OK" and makes an extended error look like it didn't fail
        lda     #DERROR
        bne     @exit                   ; always
.endproc