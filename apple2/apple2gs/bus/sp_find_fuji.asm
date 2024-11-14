                case    on              required for C compatibility
                mcopy   13:orcainclude:m16.orca assembler macros (short, long etc.)

sp_get_fuji_id	start

                ph2     #$10
                pea     sp_fuji_id
                jsl     sp_find_device_type
                bne     return          fuji device was found by id

not_found_by_id lda     sp_is_init
; if _sp_is_init is still 0, we didn't find the fujinet at all, so exit out
                beq     return

; similar to sp_find_device, but for hack check
; assumes device_count is set from previous search by ID
try_fallback    lda     #$01
                sta     sp_fuji_id

device_loop     ph2     #$03
                pha                     the current Device ID
                jsl     sp_status       sp_status(id,3) DIB request
                bne     not_found_yet there wasn't a valid DIB for this device index

; FALLBACK CHECK FOR DISK_0
; try old style, where it's the first disk that supports the status/control for fuji device - PIEPMEIER!
; the sp_payload contains following for disk_0 after a DIB status
; sp_payload[18] = $30 = '0' ascii as in "DISK_0"
; sp_payload[21] = $01 = floppy type
; sp_payload[22] = $40 = subtype

                lda     sp_payload+18
                and     #$00ff          mask high byte
                cmp     #$30            '0' ascii as in "DISK_0"
                bne     not_found_yet
                lda     sp_payload+21
                cmp     #$4001          type=$01, subtype=$40
                bne     not_found_yet

; found it, so return the current index
                lda     sp_fuji_id
                rtl

not_found_yet   inc     sp_fuji_id
                lda     sp_fuji_id
                cmp     device_count
                bcc     device_loop
                beq     device_loop

; we have checked all devices, non had the type we were looking for
                lda     #$00
                sta     sp_fuji_id
return          rtl

sp_fuji_id	entry
                dc      i2'0'

                end
