        .export         _fuji_get_adapter_config_extended
        .import         copy_fuji_cmd_data, _bus

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"
        .include        "fujinet-fuji.inc"

; void fuji_get_adapter_config_extended(AdapterConfigExtended *adapter_config)
;
.proc _fuji_get_adapter_config_extended
        ; store the memory location of the adapter config
        axinto  tmp7

        setax   #t_io_get_adapter_config_ext
        jsr     copy_fuji_cmd_data

        ; set the memory address, new size, and aux1 for DCB
        mwa     tmp7, IO_DCB::dbuflo
        jmp     _bus

.endproc

.rodata
.define ACEsz .sizeof(AdapterConfigExtended)

t_io_get_adapter_config_ext:
        .byte $e8, $40, <ACEsz, >ACEsz, $01, $00
