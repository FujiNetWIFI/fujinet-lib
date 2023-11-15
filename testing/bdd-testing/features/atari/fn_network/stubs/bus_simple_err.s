; stub BUS
;
; as bus_simple, but sets an error in DSTATS on RETURN from call

    .export     t_err_code
    .export     t_dstats

    .include    "device.inc"

    .segment    "BUS"

stubbed_bus:
    ; set value 1 in $80 for tests to detect this function was run
    lda     #$01
    sta     $80

    ; save value of dstats for testing
    lda     IO_DCB::dstats
    sta     t_dstats

    ; and write a custom error into DSTATS
    ; this will overwrite the value coming into us
    lda     t_err_code
    sta     IO_DCB::dstats
    rts

.bss
t_err_code:     .res 1
; a place to save the dstats value we received so it can be tested
t_dstats:       .res 1