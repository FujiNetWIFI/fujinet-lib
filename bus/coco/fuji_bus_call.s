* fuji_bus_call.s -- 6809 assembly, varargs via PULU
*
* bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields, ...)
*
* Varargs after fields: 0-4 aux bytes (count from fields & $07),
* then optionally data_ptr+data_len (fields & $08),
* then optionally reply_ptr+reply_len (fields & $10). Never both.
*
* U walks through arguments via PULU D. Called functions preserve U.
* S holds three saved bytes: [S]=numbytes [S+1]=fields [S+2]=header_len

        section code

        extern  _bus_ready
        extern  _dwwrite
        extern  _dwread
        extern  _fuji_get_error
        extern  _fuji_get_response
        extern  _network_get_error
        extern  _network_get_response
        extern  _fuji_field_numbytes_table

        export  _fuji_bus_call
        export  _fb_header

FUJI_DEVICEID_NETWORK       equ     $71
FUJI_DEVICEID_NETWORK_LAST  equ     $78
FUJI_DEVICEID_CLOCK         equ     $45
FUJI_DEVICEID_FUJINET       equ     $70
OP_NET                      equ     $E3
OP_CLOCK                    equ     $E5
OP_FUJI                     equ     $E2
FIELDS_DATA                 equ     $08
FIELDS_REPLY                equ     $10

_fuji_bus_call:
        pshs    u
        leau    ,s
        leau    4,u             * U = first arg (device)

        jsr     _bus_ready      * nothing in registers yet, no save/restore needed

        ldx     #_fb_header
        pulu    d               * B = device

        cmpb    #FUJI_DEVICEID_NETWORK
        blo     fbc_not_network
        cmpb    #FUJI_DEVICEID_NETWORK_LAST
        bhi     fbc_not_network

        lda     #OP_NET
        sta     ,x+
        subb    #FUJI_DEVICEID_NETWORK-1
        stb     ,x+             * unit
        pulu    d               * fuji_cmd
        stb     ,x+
        ldb     #3              * header_len
        bra     fbc_got_header

fbc_not_network:
        cmpb    #FUJI_DEVICEID_CLOCK
        beq     fbc_is_clock
        cmpb    #FUJI_DEVICEID_FUJINET
        beq     fbc_is_fuji
        clrb
        puls    u
        rts

fbc_is_clock:
        lda     #OP_CLOCK
        bra     fbc_store_opcode
fbc_is_fuji:
        lda     #OP_FUJI
fbc_store_opcode:
        sta     ,x+
        pulu    d               * fuji_cmd
        stb     ,x+
        ldb     #2              * header_len

fbc_got_header:
        pshs    b               * [S]=header_len

        pulu    d               * B = fields
        pshs    b               * [S]=fields [S+1]=header_len

        andb    #$07
        ldx     #_fuji_field_numbytes_table
        ldb     b,x             * B = numbytes
        pshs    b               * [S]=numbytes [S+1]=fields [S+2]=header_len

        beq     fbc_no_aux

        lda     2,s             * header_len
        ldx     #_fb_header
        leax    a,x             * X = first aux slot in buffer

        pulu    d               * aux1
        stb     ,x+
        ldb     ,s
        cmpb    #2
        blo     fbc_no_aux
        pulu    d               * aux2
        stb     ,x+
        ldb     ,s
        cmpb    #3
        blo     fbc_no_aux
        pulu    d               * aux3
        stb     ,x+
        ldb     ,s
        cmpb    #4
        blo     fbc_no_aux
        pulu    d               * aux4
        stb     ,x+

fbc_no_aux:
        ldb     ,s              * numbytes
        addb    2,s             * + header_len
        clra
        pshs    d               * push total length
        ldx     #_fb_header
        pshs    x
        jsr     _dwwrite
        leas    4,s

        ldb     1,s             * fields
        bitb    #FIELDS_DATA
        beq     fbc_no_data

        pulu    d               * data_ptr
        tfr     d,x
        pulu    d               * data_length
        pshs    d
        pshs    x
        jsr     _dwwrite
        leas    4,s

fbc_no_data:
        lda     >_fb_header     * opcode for dispatch

        cmpa    #OP_CLOCK
        beq     fbc_clock_reply
        cmpa    #OP_NET
        beq     fbc_net_reply

        jsr     _fuji_get_error
        tstb
        bne     fbc_return_false

        ldb     1,s             * fields
        bitb    #FIELDS_REPLY
        beq     fbc_return_true

        pulu    d               * reply_ptr
        tfr     d,x
        pulu    d               * reply_length
        pshs    d
        pshs    x
        jsr     _fuji_get_response
        leas    4,s
        bra     fbc_exit

fbc_clock_reply:
        ldb     1,s
        bitb    #FIELDS_REPLY
        beq     fbc_return_true

        pulu    d               * reply_ptr
        tfr     d,x
        pulu    d               * reply_length
        pshs    d
        pshs    x
        jsr     _dwread
        leas    4,s
        bra     fbc_exit

fbc_net_reply:
        clra
        ldb     >_fb_header+1   * unit
        pshs    d
        jsr     _network_get_error
        leas    2,s
        tstb
        bne     fbc_return_false

        ldb     1,s
        bitb    #FIELDS_REPLY
        beq     fbc_return_true

        pulu    d               * reply_ptr
        tfr     d,x
        pulu    d               * reply_length
        pshs    d
        pshs    x
        clra
        ldb     >_fb_header+1   * unit
        pshs    d
        jsr     _network_get_response
        leas    6,s
        tstb
        bne     fbc_return_false

fbc_return_true:
        ldb     #1
        bra     fbc_exit

fbc_return_false:
        clrb

fbc_exit:
        leas    3,s
        puls    u
        rts

        endsection

        section bss
_fb_header:
        rmb     7               * header (max 3) + aux (max 4)
        endsection

        end
