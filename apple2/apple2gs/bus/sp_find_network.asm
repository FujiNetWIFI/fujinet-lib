                case    on              required for C compatibility
                mcopy   13:orcainclude:m16.orca assembler macros (short, long etc.)

; uint8_t sp_get_network_id()
sp_get_network_id	start

                ph2     #$11
                pea     sp_network
                jsl     sp_find_device_type
                rtl

sp_network      entry
                dc      i2'0'

                end
