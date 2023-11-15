; stub BUS
    .include    "macros.inc"
    .include    "fujinet-io.inc"
    .include    "device.inc"
    .export     t_v

    .segment "BUS"
    .org BUS
    mwa IO_DCB::dbuflo, $80

    ldy #0
    mva t_v, {($80), y}
    rts

t_v: .byte 0
