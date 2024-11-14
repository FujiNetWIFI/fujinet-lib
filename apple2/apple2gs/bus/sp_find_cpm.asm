                case    on              required for C compatibility
                mcopy   13:orcainclude:m16.orca assembler macros (short, long etc.)

; uint8_t sp_get_cpm_id()
sp_get_cpm_id	start

                ph2     #$12
                pea     sp_cpm_id
                jsl     sp_find_device_type
                rtl

sp_cpm_id       entry
                dc      i2'0'

                end
