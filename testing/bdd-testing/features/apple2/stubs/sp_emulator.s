        .export     _setup_sp
        .export     sp_emulator

        .import     popa

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

        ; yes, so return 0 and no error, and setup payload[0] = 6
        ldy     #$00
        mva     #$06, {(ptr2), y}
        jmp     end_emulator_ok

not_DIB:
        ; device number is 1-6, convert to string we need for setting payload
        tay
        dey             ; make device number be 0 based for table lookup
        lda     DeviceNamesLo, y
        ldx     DeviceNamesHi, y
        
        jsr     set_payload
        jmp     end_emulator_ok

not_status:
; --------------------------------------------------
; OTHER COMMANDS HERE
; e.g. READ/WRITE/...

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

; AX point to string to setup, put it into payload, and save its length in there too
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

.define DeviceNames m_fn_d0, m_printer, m_network, m_clock, m_modem, m_cpm

DeviceNamesLo:     .lobytes DeviceNames
DeviceNamesHi:     .hibytes DeviceNames

; strings to set in payload
m_fn_d0:        .byte "FUJINET_DISK_0", 0
m_printer:      .byte "PRINTER", 0
m_network:      .byte "NETWORK", 0
m_clock:        .byte "CLOCK", 0
m_modem:        .byte "MODEM", 0
m_cpm:          .byte "CPM", 0
