        .export     _setup_smartport
        .export     sp_emulator
        .export     set_payload
        .export     end_emulator_ok
        .export     end_emulator_not_ok

        .export     spe_cb
        .export     spe_cmd
        .export     spe_cmdlist
        .export     spe_code
        .export     spe_dest
        .export     spe_num_devices
        .export     spe_payload
        .export     spe_should_fail_device_lookup

        .import     popa

        .include    "macros.inc"
        .include    "zp.inc"

; void setup_sp(void *callback)
;
; creates fake Smart Port at slot 1
; with a dispatcher to emulate devices it supports.
;
; The callback function is run when a non DIB command is made
; which can read the variables spe_cmd, spe_dest, spe_cmdlist, spe_payload

_setup_smartport:
        axinto  spe_cb
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

        ; setup _sp_dispatch, if we put CXFF = $06, then CX09 = _sp_dispatch address (CX00+3 + $06).
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
; _sp_dispatch routine for our SmartPort emulator
; which knows about 6 devices, detailed below.
;
sp_emulator:
        ; save the ZP values
        mva     tmp7, spe_tmp7_old
        mva     tmp8, spe_tmp8_old
        mva     tmp9, spe_tmp9_old
        mva     tmp10, spe_tmp10_old
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
        ; 4 = CONTROL
        ; 6 = OPEN
        ; 7 = CLOSE
        ; 8 = READ
        ; 9 = WRITE
        ;     All the above are passed to Callback routine established in _setup_smartport
        ;     which can access the command/dest/cmdlist/payload via exported spe_* variables
        ;

        pla                     ; low byte of return-1 (jsr pushes the address of the last byte of jsr command, not the 1 after)
        sta     tmp9
        pla                     ; high byte of return-1
        sta     tmp10

        ; COMMAND -> spe_cmd
        ldy     #$01
        lda     (tmp9), y
        sta     spe_cmd

        ; cmdlist -> spe_cmdlist
        iny
        lda     (tmp9), y       ; cmdlist low
        sta     spe_cmdlist
        iny
        lda     (tmp9), y       ; cmdlist high
        sta     spe_cmdlist+1

        ; fix the stack to point to correct return address
        adw1    tmp9, #$03

        ; store it back on the stack
        lda     tmp10
        pha
        lda     tmp9
        pha

        mwa     spe_cmdlist, tmp9

        ; dest -> spe_dest
        ldy     #$01
        mva     {(tmp9), y}, spe_dest

        ; payload -> spe_payload
        iny
        lda     (tmp9), y
        sta     spe_payload
        iny
        lda     (tmp9), y
        sta     spe_payload+1

        ; next code is statcode for STATUS, or ctrlcode for CONTROL calls
        iny
        lda     (tmp9), y
        sta     spe_code

        ; done with cmdlist, make tmp9 = payload location
        mwa     spe_payload, tmp9

        ; decide what to do with the command
        lda     spe_cmd

; --------------------------------------------------
; 0 = STATUS
        cmp     #$00
        bne     not_status

        lda     spe_dest
        ; is this the device count request?
        cmp     #$00
        bne     @not_device_count

        ; yes, so return 0 and no error, and set payload[0] = num devices
        ldy     #$00
        mva     spe_num_devices, {(tmp9), y}
        jmp     end_emulator_ok

@not_device_count:
        ; is this a DIB request (statcode == 0x03)?
        lda     spe_code
        cmp     #$03
        bne     not_DIB

        ; don't load the correct dest name device if we are told to deliberately fail device lookup        
        lda     spe_should_fail_device_lookup
        beq     @use_dest

        lda     #<m_other
        ldx     #>m_other
        jmp     @over

@use_dest:
        ; set y to the device 0-5, so we can read from table
        ldy     spe_dest
        dey

        lda     DeviceNamesLo, y
        ldx     DeviceNamesHi, y
@over:
        axinto  tmp7

        jsr     set_payload
        jmp     end_emulator_ok

not_DIB:
        ; this is a normal status request with statcode in A
        ; simply fall through to the callback as it will be expecting to handle this and command calls

not_status:
; --------------------------------------------------
; ALL OTHER COMMANDS TO CALLBACK
; (IOCTL, OPEN, CLOSE, READ, WRITE)

        ; call a generic call back routine the test can supply, it can read values from spe_* vars
        ; it must return 0 for success, 1 for error
        jsr     do_cb
        bne     end_emulator_not_ok
        ; fall through to ok case

end_emulator_ok:
        jsr     restore_tmp
        lda     #$00
        beq     :+

end_emulator_not_ok:
        pha                     ; save the error code
        jsr     restore_tmp
        pla                     ; restore error code, we want that to go to caller as a raw device error, set by test

:       ldx     #$00
        ldy     #$00
        ; this will return the caller to 3 bytes after the initial call to dispatcher
        rts

; A/X point to string to setup, put it into payload, and save its length in there too
set_payload:
        ; adjust payload (pointed to by ptr9) by 5 for string copy location
        adw1    tmp9, #$05

        ; copy from message location (tmp7) to payload
        ldy     #$00
:       lda     (tmp7), y
        beq     @end_copy
        sta     (tmp9), y
        iny
        bne     :-

@end_copy:
        sbw1    tmp9, #$05              ; reset payload pointer to correct address
        tya                             ; string length in y
        ldy     #$04
        sta     (tmp9), y               ; payload[4] = string length
        rts

do_cb:
        jmp     (spe_cb)

restore_tmp:
        mva     spe_tmp7_old, tmp7
        mva     spe_tmp8_old, tmp8
        mva     spe_tmp9_old, tmp9
        mva     spe_tmp10_old, tmp10
        rts

; --------------------------------------------------
; BSS
.bss

spe_cmd:        .res 1
spe_dest:       .res 1
spe_cmdlist:    .res 2
spe_payload:    .res 2
; doubles up as ctrlcode or statcode when it's either of those 2 commands
spe_code:       .res 1
spe_cb:         .res 2

spe_tmp7_old:   .res 1
spe_tmp8_old:   .res 1
spe_tmp9_old:   .res 1
spe_tmp10_old:  .res 1

; --------------------------------------------------
; DATA
.data

; allows tests to force emulator to not find the device they are looking for
spe_should_fail_device_lookup:  .byte 0

; allow test to override number of devices we return
spe_num_devices:                .byte 6

.define DeviceNames m_fn_d0, m_printer, m_network, m_clock, m_modem, m_cpm
DeviceNamesLo:          .lobytes DeviceNames
DeviceNamesHi:          .hibytes DeviceNames

; strings to set in payload
m_fn_d0:        .byte "FUJINET_DISK_0", 0
m_printer:      .byte "PRINTER", 0
m_network:      .byte "NETWORK", 0
m_clock:        .byte "FN_CLOCK", 0
m_modem:        .byte "MODEM", 0
m_cpm:          .byte "CPM", 0
; used when flag to fail is set
m_other:        .byte "X", 0