        .export     _main
        .export     _network_ioctl
        .export     _network_unit
        .export     _network_status_unit
        .export     _network_read

        .export     t_ioctl_dbyt
        .export     t_ioctl_dbuf
        .export     t_ioctl_dstats
        .export     t_ioctl_devspec
        .export     t_ioctl_aux1
        .export     t_ioctl_aux2
        .export     t_ioctl_cmd
        .export     t_network_unit_devicespec
        .export     t_network_status_unit
        .export     t_network_read_devicespec
        .export     t_network_read_buf
        .export     t_network_read_len
        .export     t_read_data
        .export     t_is_ioctl_error
        .export     t_is_status_error
        .export     t_is_read_error

        .export     t_devicespec
        .export     t_query
        .export     t_s

        .import     _network_json_query
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
        pushax  t_query
        setax   t_s
        jmp     _network_json_query
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

.proc _network_unit
        axinto  t_network_unit_devicespec
        jmp     return1
.endproc

.proc _network_status_unit
        sta     t_network_status_unit

        lda     t_is_status_error
        beq     :+
        jmp     return1         ; FN_ERR_IO_ERROR
:       jmp     return0         ; FN_ERR_OK
.endproc

.proc _network_read
        axinto  t_network_read_len
        popax   t_network_read_buf
        popax   t_network_read_devicespec

        lda     t_is_read_error
        beq     :+
        jmp     return1         ; FN_ERR_IO_ERROR

        ; copy string from test to the target buffer
:       mwa     #t_read_data, tmp9
        mwa     t_network_read_buf, tmp7
        ldy     #$00
:       lda     (tmp9), y
        beq     :+
        sta     (tmp7), y
        iny
        bne     :-

:       jmp     return0         ; FN_ERR_OK

.endproc

.bss

; invoker params
t_devicespec:       .res 2
t_query:            .res 2
t_s:                .res 2

; locations to ensure params are correct

t_ioctl_dbyt:       .res 2
t_ioctl_dbuf:       .res 2
t_ioctl_dstats:     .res 2
t_ioctl_devspec:    .res 2
t_ioctl_aux1:       .res 1
t_ioctl_aux2:       .res 1
t_ioctl_cmd:        .res 1

t_network_unit_devicespec:  .res 2

t_network_status_unit:      .res 1

t_network_read_devicespec:  .res 2
t_network_read_buf:         .res 2
t_network_read_len:         .res 2

; area to write text to for reading
t_read_data:        .res 32

.data
; control values

t_is_ioctl_error:   .byte 0
t_is_status_error:  .byte 0
t_is_read_error:    .byte 0
