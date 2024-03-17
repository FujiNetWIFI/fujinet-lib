; stub BUS
    .include    "macros.inc"
    .include    "device.inc"

    .segment "BUS"
    .org BUS

stubbed_bus:
    mva #$01, $80
    rts
