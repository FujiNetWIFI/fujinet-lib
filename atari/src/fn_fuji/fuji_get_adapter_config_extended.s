        .export         _fuji_get_adapter_config_extended

        .import         _bus
        .import         _fuji_success
        .import         _copy_fuji_cmd_data

        .include        "zp.inc"
        .include        "macros.inc"
        .include        "device.inc"
        .include        "fujinet-fuji.inc"

; bool fuji_get_adapter_config_extended(AdapterConfigExtended *adapter_config)
;
.proc _fuji_get_adapter_config_extended
        ; store the memory location of the adapter config
        axinto  tmp7

        setax   #t_fuji_get_adapter_config_ext
        jsr     _copy_fuji_cmd_data

        ; set the memory address, new size, and aux1 for DCB
        mwa     tmp7, IO_DCB::dbuflo
        jsr     _bus
        jmp     _fuji_success

.endproc

.rodata
.define ACEsz .sizeof(AdapterConfigExtended)

t_fuji_get_adapter_config_ext:
        .byte $c4, $40, <ACEsz, >ACEsz, $00, $00
