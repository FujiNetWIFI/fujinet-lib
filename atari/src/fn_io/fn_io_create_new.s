        .export     _fn_io_create_new

        .import     fn_io_copy_cmd_data, _fn_io_do_bus

        .include    "zp.inc"
        .include    "macros.inc"
        .include    "fujinet-io.inc"
        .include    "fn_data.inc"

; void fn_io_create_new(NewDisk *new_disk)
;
; creates a new disk from given structure
.proc _fn_io_create_new
        axinto  tmp7                    ; buffer to NewDisk

        setax   #t_io_create_new
        jsr     fn_io_copy_cmd_data
        mwa     tmp7, IO_DCB::dbuflo
        mva     #$fe, IO_DCB::dtimlo    ; high for ATX. could check the filename end and reduce this if it isn't atx
        jmp     _fn_io_do_bus
        ; implicit rts
.endproc

.rodata

.define NDsz .sizeof(NewDisk)

t_io_create_new:
        .byte $e7, $80, <NDsz, >NDsz, $00, $00
