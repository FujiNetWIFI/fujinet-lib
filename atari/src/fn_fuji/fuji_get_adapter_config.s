        .export         _fuji_get_adapter_config
        .export         t_fuji_get_adapter_config

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"
        .include        "fujinet-fuji.inc"

; bool fuji_get_adapter_config(void *adapter_config)
;
.proc _fuji_get_adapter_config
        ; store the memory location of the adapter config
        axinto  tmp7            ; adapter config location

        setax   #t_fuji_get_adapter_config
        jsr     _copy_fuji_cmd_data

        ; set the memory address for DCB to write to and call BUS
        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success

.endproc

.rodata
.define ACsz .sizeof(AdapterConfig)

t_fuji_get_adapter_config:
        .byte $e8, $40, <ACsz, >ACsz, $00, $00
