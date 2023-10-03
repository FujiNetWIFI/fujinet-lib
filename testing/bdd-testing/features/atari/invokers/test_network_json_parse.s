        .export     _main
        .export     _network_ioctl
        .export     _network_unit

        .export     t_ioctl_dbyta
        .export     t_ioctl_dbufa
        .export     t_ioctl_dstatsa
        .export     t_ioctl_devspeca
        .export     t_ioctl_aux1a
        .export     t_ioctl_aux2a
        .export     t_ioctl_cmda

        .export     t_ioctl_dbytb
        .export     t_ioctl_dbufb
        .export     t_ioctl_dstatsb
        .export     t_ioctl_devspecb
        .export     t_ioctl_aux1b
        .export     t_ioctl_aux2b
        .export     t_ioctl_cmdb

        .export     t_network_unit_devicespec
        .export     t_is_ioctl_error

        .export     t_devicespec
        .export     t_mode

        .import     _network_json_parse
        .import     popa
        .import     popax
        .import     pushax
        .import     return0
        .import     return1

        .include    "macros.inc"
        .include    "zp.inc"

; Invoke the function under test.
.proc _main
        setax   t_devicespec
        jmp     _network_json_parse
.endproc

; MOCK implemenations
; these will save the args that were called, then return ok/err depending on what the test requires

; this is called twice for JSON channel mode, and to perform the Query, so capture both invocations
.proc _network_ioctl
        inc     t_ioctl_call_count
        ldy     t_ioctl_call_count
        cpy     #$01
        bne     r2
r1:
        axinto  t_ioctl_dbyta
        popax   t_ioctl_dbufa
        popax   t_ioctl_dstatsa
        popax   t_ioctl_devspeca
        popa    t_ioctl_aux2a
        popa    t_ioctl_aux1a
        popa    t_ioctl_cmda
        jmp     over
r2:
        axinto  t_ioctl_dbytb
        popax   t_ioctl_dbufb
        popax   t_ioctl_dstatsb
        popax   t_ioctl_devspecb
        popa    t_ioctl_aux2b
        popa    t_ioctl_aux1b
        popa    t_ioctl_cmdb

over:
        lda     t_is_ioctl_error
        beq     :+
        jmp     return1         ; FN_ERR_IO_ERROR
:       jmp     return0         ; FN_ERR_OK

.endproc

.proc _network_unit
        axinto  t_network_unit_devicespec
        jmp     return1
.endproc

.bss

; invoker params
t_devicespec:       .res 2
t_mode:             .res 1

; locations to ensure params are correct

t_ioctl_dbyta:       .res 2
t_ioctl_dbufa:       .res 2
t_ioctl_dstatsa:     .res 2
t_ioctl_devspeca:    .res 2
t_ioctl_aux1a:       .res 1
t_ioctl_aux2a:       .res 1
t_ioctl_cmda:        .res 1

t_ioctl_dbytb:       .res 2
t_ioctl_dbufb:       .res 2
t_ioctl_dstatsb:     .res 2
t_ioctl_devspecb:    .res 2
t_ioctl_aux1b:       .res 1
t_ioctl_aux2b:       .res 1
t_ioctl_cmdb:        .res 1

t_network_unit_devicespec:  .res 2

.data
; control values

t_is_ioctl_error:   .byte 0
t_ioctl_call_count: .byte 0