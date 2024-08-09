                case    on              Required for C compatibility

sp_data         data
                copy    apple2:src:include:sp.equ

; has the init routine been run?
sp_is_init      entry
                dc i1'0'

; the network sub-device id matching Nx: ranging from 1 to 8. 0 means it hasn't been specified in an open.
sp_nw_unit      entry
                dc i1'0'

sp_dest         entry
                ds 1
sp_error        entry
                ds 1
sp_count        entry
                ds 2

sp_payload      entry
                ds SP_PAYLOAD_SIZE

MyID            entry
                ds 2

sp_fwdata       entry
                dc i4'0'                

                end
