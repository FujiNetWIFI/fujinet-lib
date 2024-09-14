        .export         _sp_find_device

        .export         device_type_id
        .export         device_count
        .export         device_id_idx
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

; int sp_find_device(uint8_t type_id);
; where type_id is the internal fujinet device type_id:
;  $10 = fujinet
;  $11 = network
;  $12 = cpm
;  $13 = clock
;  $14 = printer
;  $15 = modem
;  $01 = floppy disk
;  $02 = hard disk
_sp_find_device:
        sta     device_type_id          ; save the type_id
        ; check if we've already run sp_init
        lda     _sp_is_init
        bne     have_init

        ; no, so do it now, we first have to save the current type we're searching for, as it gets overwritten searching for network device
        lda     device_type_id
        sta     tmp_orig_type
        jsr     _sp_init
        bne     restore_type

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

        ; there was an error, negate A/X and return it
        jmp     negax
        ; implicit RTS

status_ok_1:
        ldx     #$00                    ; prep the return value if it's going to be 0
        lda     _sp_payload             ; device count in sp_payload[0]
        beq     :+                      ; if the count is zero, just return 0, don't touch X as it's already 0
        bpl     have_count

        ; byte extend the negative value into X, as A is already negative
        dex
:       rts

have_count:
        ; now repeatedly call sp_status(i, 3) to get the DIB status for the device, which contains name and its device type
        ; a contains the count of devices, 1 based
        sta     device_count

        lda     #$01
        sta     device_id_idx
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
        lda     device_id_idx
        rts

not_found_yet:
        inc     device_id_idx
        lda     device_id_idx
        cmp     device_count
        bcc     device_loop
        beq     device_loop

        ; we have checked all devices, non had the type we were looking for
        jmp     return0


.bss
device_type_id: .res 1
device_count:   .res 1
device_id_idx:  .res 1
tmp_orig_type:  .res 1