; stub BUS
    .export     info

    .include    "macros.inc"
    .include    "fujinet-io.inc"
    .include    "fn_data.inc"

    .segment "BUS"
    .org BUS

    ; copy data into "info"
    mwa IO_DCB::dbuflo, $80
    ldy #8
:   mva {t_ssid, y}, {info + SSIDInfo::ssid, y}
    dey
    bpl :-
    mva t_rssi, {info + SSIDInfo::rssi}

    ; copy the test ssidinfo to the caller's buffer.
    ldy #34
:   mva {info, y}, {($80), y}
    dey
    bpl :-

    rts

t_ssid:  .byte "ssidtime"
t_rssi:  .byte $69
info:    .tag SSIDInfo
