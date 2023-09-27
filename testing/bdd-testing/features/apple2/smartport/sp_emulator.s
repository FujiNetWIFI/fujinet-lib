        .export     _setup_sp
        .export     sp_emulator

        .import     popa

        .import     close_fnd0
        .import     close_printer
        .import     close_network
        .import     close_clock
        .import     close_modem
        .import     close_cpm
        .import     control_fnd0
        .import     control_printer
        .import     control_network
        .import     control_clock
        .import     control_modem
        .import     control_cpm
        .import     open_fnd0
        .import     open_printer
        .import     open_network
        .import     open_clock
        .import     open_modem
        .import     open_cpm
        .import     read_fnd0
        .import     read_printer
        .import     read_network
        .import     read_clock
        .import     read_modem
        .import     read_cpm
        .import     write_fnd0
        .import     write_printer
        .import     write_network
        .import     write_clock
        .import     write_modem
        .import     write_cpm

        .include    "macros.inc"
        .include    "zp.inc"

; void setup_sp()
;
; creates fake Smart Port at slot 1
; with a dispatcher to emulate devices it supports.

_setup_sp:
        mwa     #$c100, ptr1

        ; SP detection needs:
        ;  CX01 = $20
        ;  CX03 = $00
        ;  CX05 = $03
        ;  CX07 = $00
        ldy     #$01
        mva     #$20, {(ptr1), y}

        ldy     #$03
        mva     #$00, {(ptr1), y}

        ldy     #$05
        mva     #$03, {(ptr1), y}

        ldy     #$07
        mva     #$00, {(ptr1), y}

        ; setup dispatch, if we put CXFF = $06, then CX09 = dispatch address (CX00+3 + $06).
        ldy     #$ff
        mva     #$06, {(ptr1), y}

        ; create a "JMP sp_emulator" at CX09
        ldy     #$09
        mva     #$4C, {(ptr1), y}       ; JMP
        iny
        ; address of dispatcher
        mway    #sp_emulator, {(ptr1), y}

        ; exit our setup code
        rts

; -------------------------------------------------------------
; Dispatch routine for our fake SmartPort
; which knows about 6 devices, detailed below.
sp_emulator:
        ; The caller invokes dispatcher with:
        ;   jsr dispatcher
        ;   db command
        ;   dw sp_cmdList
        ;
        ; where sp_cmdList holds:
        ;   db command_count    - number of bytes in the cmdList that are relevant
        ;   db dest             - see table below for dest/unit values. 1 == fujinet, etc.
        ;   dw sp_payload       - always address of payload array (1024)
        ;   ... various additional bytes depending on the command
        ;
        ; commands:
        ; 0 = status
        ;     if dest == 0, statcode == 0, return 0 (num bytes) and no error, payload[0] = 6 == num devices
        ;     if dest == n (unit), statcode == 3
        ;       return info according to unit being asked for:
        ;         FUJINET_DISK_0 -> unit = 1, pl[4] = 14, pl[5..18] = name : sp_dest = 1 for fujinet device in tests
        ;                PRINTER -> unit = 2, pl[4] = 7,  pl[5..11] = name
        ;                NETWORK -> unit = 3, pl[4] = 7,  pl[5..11] = name
        ;               FN_CLOCK -> unit = 4, pl[4] = 8,  pl[5..12] = name
        ;                  MODEM -> unit = 5, pl[4] = 5,  pl[5..9]  = name
        ;                    CPM -> unit = 6, pl[4] = 3,  pl[5..7]  = name
        ;
        ; statcode = FN command code, e.g. 0xFA = Get Wifi Status.
        ; thus we can detect a request for FN command via sp_status(1, 0xFA), etc.

        pla                     ; low byte of return-1
        sta     tmp9
        pla                     ; high byte of return-1
        sta     tmp10

        ; COMMAND -> tmp1
        ldy     #$01
        lda     (tmp9), y
        sta     tmp1

        ; cmdlist -> ptr1
        iny
        lda     (tmp9), y       ; cmdlist low
        sta     ptr1
        iny
        lda     (tmp9), y       ; cmdlist high
        sta     ptr1+1

        ; fix the stack to point to correct return address
        adw1    tmp9, #$03

        ; store it back on the stack
        lda     tmp10
        pha
        lda     tmp9
        pha

        ; dest -> tmp2
        ldy     #$01
        mva     {(ptr1), y}, tmp2

        ; payload -> ptr2
        iny
        lda     (ptr1), y
        sta     ptr2
        iny
        lda     (ptr1), y
        sta     ptr2+1

        ; decide what to do with the callers args

        ; set y to the device 0-5
        lda     tmp2
        tay
        dey

        lda     tmp1
;--------------------------------------------------------------------------
; COMMAND SWITCH
;--------------------------------------------------------------------------

; --------------------------------------------------
; 0 = STATUS
        cmp     #$00
        bne     not_status

        lda     tmp2
        ; is this the DIB request?
        cmp     #$00
        bne     not_DIB

        ; yes, so return 0 and no error, and set payload[0] = 6 for 6 devices
        ldy     #$00
        mva     #$06, {(ptr2), y}
        jmp     end_emulator_ok

not_DIB:
        lda     DeviceNamesLo, y
        ldx     DeviceNamesHi, y
        
        jsr     set_payload
        jmp     end_emulator_ok

not_status:
; --------------------------------------------------
; 4 = CONTROL
        cmp     #$04
        bne     not_control

        ; this is typically the FN command to issue, and get a response from
        lda     ControlFunctionsLo, y
        ldx     ControlFunctionsHi, y
        jmp     redirect

not_control:
; --------------------------------------------------
; 6 = OPEN
        cmp     #$06
        bne     not_open

        lda     OpenFunctionsLo, y
        ldx     OpenFunctionsHi, y
        jmp     redirect

not_open:
; --------------------------------------------------
; 7 = CLOSE
        cmp     #$07
        bne     not_close

        lda     CloseFunctionsLo, y
        ldx     CloseFunctionsHi, y
        jmp     redirect

not_close:
; --------------------------------------------------
; 8 = READ
        cmp     #$08
        bne     not_read

        lda     ReadFunctionsLo, y
        ldx     ReadFunctionsHi, y
        jmp     redirect

not_read:
; --------------------------------------------------
; 9 = WRITE
        cmp     #$09
        bne     not_write

        lda     WriteFunctionsLo, y
        ldx     WriteFunctionsHi, y
        jmp     redirect

not_write:
; ... any more commands should be implemented here.

        bne     end_emulator_not_ok

end_emulator_ok:
        lda     #$00
        .byte   $2c             ; BIT, ignore next 2 bytes
end_emulator_not_ok:
        lda     #$01

        ldx     #$00
        ldy     #$00
        ; this will return the caller to 3 bytes after the initial call to dispatcher
        rts

; A/X hold function address to redirect to
redirect:
        axinto  ptr2                    ; function to call
        setax   ptr1                    ; pass cmdlist to control function, it can read everything from it
        jsr     run_fn                  ; can't do jsr (ptr2), so have to do it via run_fn

        beq     end_emulator_ok
        bne     end_emulator_not_ok


; A/X point to string to setup, put it into payload, and save its length in there too
set_payload:
        axinto  ptr3
        ; decrease ptr3 (pointer to string to save) by 5 so the y index matches copy
        sbw1    ptr3, #$05
        ldy     #$05            ; payload offset for string [5..5+len]
:       lda     (ptr3), y
        beq     @end_copy
        sta     (ptr2), y
        iny
        bne     :-

@end_copy:
        ; decrease y by 5 to get string length
        tya
        sec
        sbc     #$05            ; a = string length
        ldy     #$04            ; payload offset for length
        sta     (ptr2), y       ; payload[4] = string length
        rts

run_fn:
        jmp     (ptr2)

.define DeviceNames m_fn_d0, m_printer, m_network, m_clock, m_modem, m_cpm
DeviceNamesLo:          .lobytes DeviceNames
DeviceNamesHi:          .hibytes DeviceNames

.define ControlFunctions control_fnd0, control_printer, control_network, control_clock, control_modem, control_cpm
ControlFunctionsLo:     .lobytes ControlFunctions
ControlFunctionsHi:     .hibytes ControlFunctions

.define OpenFunctions open_fnd0, open_printer, open_network, open_clock, open_modem, open_cpm
OpenFunctionsLo:     .lobytes OpenFunctions
OpenFunctionsHi:     .hibytes OpenFunctions

.define CloseFunctions close_fnd0, close_printer, close_network, close_clock, close_modem, close_cpm
CloseFunctionsLo:     .lobytes CloseFunctions
CloseFunctionsHi:     .hibytes CloseFunctions

.define ReadFunctions read_fnd0, read_printer, read_network, read_clock, read_modem, read_cpm
ReadFunctionsLo:     .lobytes ReadFunctions
ReadFunctionsHi:     .hibytes ReadFunctions

.define WriteFunctions write_fnd0, write_printer, write_network, write_clock, write_modem, write_cpm
WriteFunctionsLo:     .lobytes WriteFunctions
WriteFunctionsHi:     .hibytes WriteFunctions

; strings to set in payload
m_fn_d0:        .byte "FUJINET_DISK_0", 0
m_printer:      .byte "PRINTER", 0
m_network:      .byte "NETWORK", 0
m_clock:        .byte "FN_CLOCK", 0
m_modem:        .byte "MODEM", 0
m_cpm:          .byte "CPM", 0
