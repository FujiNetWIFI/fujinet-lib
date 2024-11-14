                case    on              required for C compatibility
                mcopy   13:orcainclude:m16.orca assembler macros (short, long etc.)

; uint8_t sp_get_printer_id()
sp_get_printer_id	start

                ph2     #$14
                pea     sp_printer_id
                jsl     sp_find_device_type
                rtl

sp_printer_id   entry
                dc      i2'0'

                end
