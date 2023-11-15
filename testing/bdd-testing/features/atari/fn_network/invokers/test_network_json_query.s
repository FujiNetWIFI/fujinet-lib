        .export     _main
        .export     _network_ioctl
        .export     _network_status_unit
        .export     _network_unit
        .export     _sio_read

        .export     t_ioctl_dbyt
        .export     t_ioctl_dbuf
        .export     t_ioctl_dstats
        .export     t_ioctl_devspec
        .export     t_ioctl_aux1
        .export     t_ioctl_aux2
        .export     t_ioctl_cmd
        .export     t_network_unit_devicespec
        .export     t_network_status_unit
        .export     t_network_status_bw
        .export     t_network_status_conn
        .export     t_network_status_err
        .export     t_sio_read_unit
        .export     t_sio_read_buf
        .export     t_sio_read_len
        .export     t_read_data
        .export     t_is_ioctl_error
        .export     t_is_status_error
        .export     t_is_read_error

        .export     t_devicespec
        .export     t_query
        .export     t_s

        .import     _fn_bytes_read
        .import     _fn_network_bw
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

.proc _network_unit
        axinto  t_network_unit_devicespec
        jmp     return1
.endproc

.proc _network_ioctl
        popax   t_ioctl_dbyt
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

.proc _network_status_unit
        axinto  t_network_status_err
        popax   t_network_status_conn
        popax   t_network_status_bw
        popa    t_network_status_unit

        mwa     #$07, _fn_network_bw

        lda     t_is_status_error
        beq     :+
        jmp     return1         ; FN_ERR_IO_ERROR
:       jmp     return0         ; FN_ERR_OK
.endproc

.proc _sio_read
        axinto  t_sio_read_len
        popax   t_sio_read_buf
        popa    t_sio_read_unit

        mva     tmp7, t_tmp7
        mva     tmp8, t_tmp8
        mva     tmp9, t_tmp9
        mva     tmp10, t_tmp10

        lda     t_is_read_error
        beq     :+
        jmp     return1         ; FN_ERR_IO_ERROR

        ; copy string from test to the target buffer
:       mwa     #t_read_data, tmp9
        mwa     t_sio_read_buf, tmp7
        ldy     #$00
:       lda     (tmp9), y
        beq     :+
        sta     (tmp7), y
        iny
        bne     :-

:       mwa     t_sio_read_len, _fn_bytes_read
        mva     t_tmp7, tmp7
        mva     t_tmp8, tmp8
        mva     t_tmp9, tmp9
        mva     t_tmp10, tmp10

        jmp     return0         ; FN_ERR_OK

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

t_network_status_unit:  .res 1
t_network_status_bw:    .res 2
t_network_status_conn:  .res 2
t_network_status_err:   .res 2

t_sio_read_unit:        .res 1
t_sio_read_buf:         .res 2
t_sio_read_len:         .res 2

; area to write text to for reading
t_read_data:    .res 32

t_tmp7:         .res 1
t_tmp8:         .res 1
t_tmp9:         .res 1
t_tmp10:        .res 1

.data
; control values

t_is_ioctl_error:   .byte 0
t_is_status_error:  .byte 0
t_is_read_error:    .byte 0
