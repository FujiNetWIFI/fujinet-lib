        .export         sp_find_device_type

        .export         device_type_id
        .export         device_count
        .export         _device_id_idx
        .export         tmp_orig_type

        .import         _sp_cmdlist
        .import         _sp_init
        .import         _sp_is_init
        .import         _sp_nw_unit
        .import         _sp_payload
        .import         _sp_status

        .import         pusha
        .import         negax
        .import         return0

        .include        "sp.inc"
        .include        "zp.inc"

; uint8_t sp_find_device();
; device_type_id contains the type id to search for
; where type_id is the internal fujinet device type_id:
;  $10 = fujinet
;  $11 = network
;  $12 = cpm
;  $13 = clock
;  $14 = printer
;  $15 = modem
;  $01 = floppy disk
;  $02 = hard disk
sp_find_device:
        ; check if we've already run sp_init
        lda     _sp_is_init
        bne     have_init

        ; no, so do it now, we first have to save the current type we're searching for, as it gets overwritten searching for network device
        lda     device_type_id
        sta     tmp_orig_type
        jsr     _sp_init
        bne     restore_type

return_error:
        ; not found, so return 0 as the network device, and is an error
        jmp     return0

restore_type:
        lda     tmp_orig_type
        sta     device_type_id

have_init:
        lda     #$00
        jsr     pusha                   ; doesn't change A, so can be used to double up as both params
        jsr     _sp_status              ; sp_status(0,0) fetches the device count

        beq     status_ok_1
        bne     return_error

status_ok_1:
        lda     _sp_payload             ; device count in sp_payload[0]
        beq     return_error            ; if the count is <= 0, just return 0
        bmi     return_error

have_count:
        ; now repeatedly call sp_status(i, 3) to get the DIB status for the device, which contains name and its device type
        ; a contains the count of devices, 1 based
        sta     device_count

        lda     #$01
        sta     _device_id_idx
device_loop:
        jsr     pusha                   ; the current Device ID
        lda     #$03
        jsr     _sp_status              ; sp_status(id,3) DIB request

        bne     not_found_yet           ; there wasn't a valid DIB for this device index

        ; compare sp_payload[21] to device_type_id
        lda     _sp_payload+21
        cmp     device_type_id
        bne     not_found_yet

        ; found it, so return the current index
        ldx     #$00
        lda     _device_id_idx
        rts

not_found_yet:
        inc     _device_id_idx
        lda     _device_id_idx
        cmp     device_count
        bcc     device_loop
        beq     device_loop

        ; we have checked all devices, non had the type we were looking for
        bcs     return_error

; A = device type (e.g. $11 = network)
; X/Y = location to save the id
sp_find_device_type:
        sta     device_type_id          ; the type to search for
        stx     id_loc1                 ; the low location of the address to save the id at
        sty     id_loc1 + 1             ; the high location of the address to save the id at

        lda     $ffff                   ; don't use 0000, compiler thinks you want ZP!
id_loc1 = *-2

        ; is it already set?
        beq     :+

        ; the id was set in the indicated location, and is now in A, so return it, setting X to 0 for high byte for C callers
        ldx     #$00
        cmp     #$00                    ; force status bits for the return
        rts

        ; no ID set, so fetch it, write x/y to the location the id needs to be saved to
:       stx     id_loc2
        sty     id_loc2 + 1

        jsr     sp_find_device

        beq     not_found

        ; found it from searching, so return the id after setting it
        jsr     set_id
        ldx     #$00
        cmp     #$00                    ; set status registers for the return based on A's id value
        rts

not_found:
        tax                             ; set x to 0

set_id:
        sta     $ffff
id_loc2 = *-2

        rts

.bss
device_type_id: .res 1
device_count:   .res 1
_device_id_idx:  .res 1
tmp_orig_type:  .res 1