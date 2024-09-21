        .export         _sp_control
        .export         _sp_control_nw
        .export         sp_dispatch
        .export         _sp_dispatch_address
        .export         _sp_status
        .export         _sp_status_nw

        .import         _sp_cmdlist
        .import         _sp_nw_unit
        .import         _sp_payload
        .import         _sp_count
        .import         _sp_error

        .import         popa

        .include        "sp.inc"

; int8_t sp_control(uint8_t dest, uint8_t ctrlcode)
_sp_control:
        ldy     #SP_CONTROL_PARAM_COUNT
        ldx     #SP_CMD_CONTROL
        bne     do_common

; int8_t sp_control_nw(uint8_t dest, uint8_t ctrlcode)
_sp_control_nw:
        pha
        lda     _sp_nw_unit
        sta     _sp_cmdlist+5                   ; sp_cmdlist[5] = sp_nw_unit;
        pla

        ldy     #SP_CONTROL_PARAM_COUNT_NW
        ldx     #SP_CMD_CONTROL
        bne     do_common

; int8_t sp_status_nw(uint8_t dest, uint8_t statcode)
_sp_status_nw:
        pha
        lda     _sp_nw_unit
        sta     _sp_cmdlist+5                   ; sp_cmdlist[5] = sp_nw_unit;
        pla

        ldy     #SP_STATUS_PARAM_COUNT_NW
        ldx     #SP_CMD_STATUS
        beq     do_common

; int8_t sp_status(uint8_t dest, uint8_t statcode)
; this is called quite often, so make it the fall through case
_sp_status:
        ldy     #SP_STATUS_PARAM_COUNT
        ldx     #SP_CMD_STATUS
        ; fall through

do_common:
        sty     _sp_cmdlist                     ; sp_cmdlist[0] = count for particular command
        sta     _sp_cmdlist+4                   ; sp_cmdlist[4] = <CODE>

        jsr     popa                            ; doesn't affect X, get destination value from s/w stack
        sta     _sp_cmdlist+1                   ; sp_cmdlist[1] = dest;
        lda     #<(_sp_payload)
        sta     _sp_cmdlist+2                   ; sp_cmdlist[2] = payload_address & 0xFF;
        lda     #>(_sp_payload)
        sta     _sp_cmdlist+3                   ; sp_cmdlist[3] = payload_address >> 8;

sp_dispatch:
        ; X contains the CONTROL or STATUS command to send
        stx     dispatch_data
        lda     #<_sp_cmdlist
        sta     dispatch_data+1
        lda     #>_sp_cmdlist
        sta     dispatch_data+2
        
        ; the SP dispatch alters the return address by 3 bytes to skip the data below.
        ; it returs with any error codes.
        ; The dispatch address is altered in sp_init
        jsr     $ffff
_sp_dispatch_address = *-2

dispatch_data:
        .byte   $00             ; command
        .byte   $00             ; cmdlist low
        .byte   $00             ; cmdlist high

        ; A is error code. X,Y are the count of bytes
        sta     _sp_error
        stx     _sp_count
        sty     _sp_count+1

        ; convert to a return value
        ldx     #$00
        lda     _sp_error
        rts

