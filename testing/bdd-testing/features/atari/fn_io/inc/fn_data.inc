; test entry point for all BUS calls, also defined in the cfg file so that any "BUS" segment compiles to same address
BUS    = $2000

.struct IO_DCB
    .org    $1000
    ddevic  .byte
    dunit   .byte
    dcomnd  .byte
    dstats  .byte
    dbuflo  .byte
    dbufhi  .byte
    dtimlo  .byte
    dunuse  .byte
    dbytlo  .byte
    dbythi  .byte
    daux1   .byte
    daux2   .byte
.endstruct
