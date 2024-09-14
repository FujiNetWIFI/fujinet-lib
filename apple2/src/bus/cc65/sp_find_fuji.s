        .export         _sp_get_fuji_id
        .export         _sp_fuji_id

        .import         _sp_find_device
        .import         _sp_is_init
        .import         _sp_payload
        .import         _sp_status

        .import         device_id_idx
        .import         device_count

        .import         pusha
        .import         return0
        .import         return1

        .macpack        cpu

sp_find_fuji:
        lda     #$10
        jsr     _sp_find_device

        bmi     not_found_by_id
        beq     not_found_by_id

        sta     _sp_fuji_id
        jmp     return1

not_found_by_id:
        ; if _sp_is_init is still 0, we didn't find the fujinet at all, so exit out
        lda     _sp_is_init
        beq     :+

        ; we need to loop through all the devices again for JEFF'S HACK
        jsr     try_fallback

        ; either ID is in A, or it's 0 for failure, either way, we use that to set fuji id and return
:       sta     _sp_fuji_id
        rts

; uint8_t sp_get_fuji_id()
_sp_get_fuji_id:
        ldx     #$00                    ; prep the return hi byte for C callers
        lda     _sp_fuji_id
        bne     :+                      ; if it's already set, just exit

        ; otherwise we need to try and find it from SP
        jsr     sp_find_fuji

        ; return whatever was set in sp_fuji
        ldx     #$00
        lda     _sp_fuji_id
:       rts


; similar to sp_find_device, but for hack check
; assumes device_count is set from previous search by ID
try_fallback:

        lda     #$01
        sta     device_id_idx
device_loop:
        jsr     pusha                   ; the current Device ID
        lda     #$03
        jsr     _sp_status              ; sp_status(id,3) DIB request

        bne     not_found_yet           ; there wasn't a valid DIB for this device index


        ; FALLBACK CHECK FOR DISK_0
        ; try old style, where it's the first disk that supports the status/control for fuji device - PIEPMEIER!
        ; the sp_payload contains following for disk_0 after a DIB status
        ; sp_payload[18] = $30 = '0' ascii as in "DISK_0"
        ; sp_payload[21] = $01 = floppy type
        ; sp_payload[22] = $40 = subtype

        lda     _sp_payload+18
        cmp     #$30
        bne     not_found_yet
        lda     _sp_payload+21
        cmp     #$01
        bne     not_found_yet
        lda     _sp_payload+22
        cmp     #$40
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


.data
_sp_fuji_id:   .byte $00
