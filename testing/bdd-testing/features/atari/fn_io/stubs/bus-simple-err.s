; stub BUS
    .export     t_bus_err

    .include    "macros.inc"
    .include    "device.inc"

    .segment    "BUS"
    .org        BUS

    ; marks the bus as having run, but returns the error code indicated in t_bus_err
    ; to allow test to return an error code from the BUS call

stubbed_bus:
    mva     #$01, $80
    ldx     #$00
    lda     t_bus_err
    sta     IO_DCB::dstats
    rts

t_bus_err: .res 1
