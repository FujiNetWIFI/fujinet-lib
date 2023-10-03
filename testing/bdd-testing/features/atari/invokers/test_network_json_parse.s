        .export     _main
        .export     _network_ioctl

        .export     t_ioctl_dbyt
        .export     t_ioctl_dbuf
        .export     t_ioctl_dstats
        .export     t_ioctl_devspec
        .export     t_ioctl_aux1
        .export     t_ioctl_aux2
        .export     t_ioctl_cmd
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
        pushax  t_devicespec
        lda     t_mode
        jmp     _network_json_parse
.endproc

; MOCK implemenations
; these will save the args that were called, then return ok/err depending on what the test requires

.proc _network_ioctl
        axinto  t_ioctl_dbyt
        popax   t_ioctl_dbuf
        popax   t_ioctl_dstats
        popax   t_ioctl_devspec
        popa    t_ioctl_aux2
        popa    t_ioctl_aux1
        popa    t_ioctl_cmd

        lda     t_is_ioctl_error
        beq     :+
        jmp     return1         ; FN_ERR_IO_ERROR
:       jmp     return0         ; FN_ERR_OK

.endproc

.bss

; invoker params
t_devicespec:       .res 2
t_mode:             .res 1

; locations to ensure params are correct

t_ioctl_dbyt:       .res 2
t_ioctl_dbuf:       .res 2
t_ioctl_dstats:     .res 2
t_ioctl_devspec:    .res 2
t_ioctl_aux1:       .res 1
t_ioctl_aux2:       .res 1
t_ioctl_cmd:        .res 1

.data
; control values

t_is_ioctl_error:   .byte 0
