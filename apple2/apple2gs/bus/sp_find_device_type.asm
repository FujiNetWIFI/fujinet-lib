                case    on              required for C compatibility
                mcopy   13:ainclude:m16.cc
                mcopy   13:orcainclude:m16.orca assembler macros (short, long etc.)

; A = device type (e.g. $11 = network)
; X/Y = location to save the id
sp_find_device_type start
                csubroutine (2:id_loc,2:device_type_id),2

device_id_idx   equ     1

                lda     (id_loc)
                sta     device_id_idx
; is it already set?
                bne     return
; no ID set, so fetch it, write x/y to the location the id needs to be saved to
                jsr     sp_find_device
; found it from searching, so return success after setting the id
                sta     (id_loc)

return          creturn 2:device_id_idx


; bool sp_find_device();
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
sp_find_device  lda     sp_is_init      check if we've already run sp_init
                bne     have_init
                jsl     sp_init
                beq     return_error

have_init       ph2     #$00
                ph2     #0
                jsl     sp_status       sp_status(0,0) fetches the device count
                bne     return_0

status_ok_1     short m
                lda     sp_payload
                long m
                beq     return_error
                bmi     return_0

; now repeatedly call sp_status(i, 3) to get the DIB status for the device, which contains name and its device type
; a contains the count of devices, 1 based
have_count      sta     device_count

                lda     #1
                sta     device_id_idx
device_loop     ph2     #$03
                pha
                jsl     sp_status       sp_status(id,3) DIB request
                bne     not_found_yet   there wasn't a valid DIB for this device index

; compare sp_payload[21] to device_type_id
                short   m
                lda     sp_payload+21
                long    m
                cmp     device_type_id
                bne     not_found_yet

; found it, so return the current index
                lda     device_id_idx
                rts

not_found_yet   inc     device_id_idx
                lda     device_id_idx
                cmp     device_count
                bcc     device_loop
                beq     device_loop

; we have checked all devices, none had the type we were looking for
return_0        lda     #0
                sta     device_id_idx
return_error    rts

device_count    entry
                ds      2

                end
