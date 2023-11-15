; stub BUS
;
; The BUS is just a concept for platforms that perform some FN call.
; In the test framework, it's a fake area of memory defined in device.inc.
; On real platforms it would be an appropriate location to call its BUS equivalent.

    .include    "device.inc"

    .segment "BUS"

stubbed_bus:
    ; simply set value 1 in $80 for tests to detect this function was run
    lda     #$01
    sta     $80
    rts
