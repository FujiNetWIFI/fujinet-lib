        .export         _fn_io_get_adapter_config
        .export         t_io_get_adapter_config
        .import         copy_io_cmd_data, _bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"
        .include        "fujinet-io.inc"

; void fn_io_get_adapter_config(void *adapter_config)
;
.proc _fn_io_get_adapter_config
        ; store the memory location of the adapter config
        axinto  tmp7            ; adapter config location

        setax   #t_io_get_adapter_config
        jsr     copy_io_cmd_data

        ; set the memory address for DCB to write to and call BUS
        mwa     tmp7, IO_DCB::dbuflo
        jmp     _bus

.endproc

.rodata
.define ACsz .sizeof(AdapterConfig)

t_io_get_adapter_config:
        .byte $e8, $40, <ACsz, >ACsz, $00, $00
