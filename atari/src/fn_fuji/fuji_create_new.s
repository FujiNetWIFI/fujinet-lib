        .export     _fuji_create_new

        .import     _bus
        .import     _fuji_success
        .import     copy_fuji_cmd_data

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "fujinet-fuji.inc"
        .include    "device.inc"

; bool fuji_create_new(NewDisk *new_disk)
;
; creates a new disk from given structure
.proc _fuji_create_new
        axinto  tmp7                    ; buffer to NewDisk

        setax   #t_fuji_create_new
        jsr     copy_fuji_cmd_data
        mwa     tmp7, IO_DCB::dbuflo
        mva     #$fe, IO_DCB::dtimlo    ; high for ATX. could check the filename end and reduce this if it isn't atx
        jsr     _bus
        jmp     _fuji_success
.endproc

.rodata

.define NDsz .sizeof(NewDisk)

t_fuji_create_new:
        .byte $e7, $80, <NDsz, >NDsz, $00, $00
