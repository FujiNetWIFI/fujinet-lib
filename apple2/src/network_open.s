        .export     _network_open

        .import     _bzero
        .import     _fn_device_error
        .import     _memcpy
        .import     _sp_clr_pay
        .import     _sp_control
        .import     _sp_find_network
        .import     _sp_init
        .import     _sp_open
        .import     _sp_network
        .import     _sp_payload
        .import     _strlen
        .import     _strncpy
        .import     fn_open_mode
        .import     incsp3
        .import     popa
        .import     popax
        .import     pusha
        .import     pushax
        .import     return1

        .include    "sp.inc"
        .include    "macros.inc"
        .include    "zp.inc"

init_error:
        ; set io error
        lda     #SP_ERR_IO_ERROR
        ; fall through

remove_params_return_error:
        sta     _fn_device_error
        ; need to move SP on 3 bytes to skip unread args
        jsr     incsp3
        jmp     return1         ; FN_ERR_IO_ERROR

sp_error:
        ; smart port error (negative value in a)
        eor     #$ff
        clc
        adc     #$01
        ; never 0, as we detected the error before calling sp_error
        bne     remove_params_return_error

; uint8_t network_open(char* devicespec, uint8_t mode, uint8_t trans);
_network_open:
        sta     tmp4            ; trans

        jsr     _sp_clr_pay     ; calls bzero, so trashes p1/2/3
        ldy     #$00
        sty     _fn_device_error

        jsr     _sp_init
        beq     init_error      ; didn't find a FN device at all
        jsr     _sp_find_network
        beq     init_error      ; didn't find a network device on the smartport
        bmi     sp_error        ; actual device error, but negative so we can distinguish between device count and an error

        sta     _sp_network     ; keep track of network unit for other functions
        jsr     _sp_open
        bne     remove_params_return_error      ; failed to call open, SmartPort error in A (not negative)

        jsr     popa            ; mode
        sta     _sp_payload+2   ; save mode into payload (e.g. $c for r/w)

        ; save mode in our global variable as it's used in several places
        sta     fn_open_mode

        popax   ptr1            ; devicespec
        jsr     _strlen
        axinto  ptr4            ; ptr4 hold string len

        ; add 2 and store resultant length+2 in sp_payload[0,1]. this is the length of bytes to read from payload, so includes 2 for trans/mode
        adw     ptr4, #$02, _sp_payload

        mva     tmp4, _sp_payload+3     ; translation

        ; copy the devicespec to payload[4], and add on 0x00
        pushax  #_sp_payload+4  ; dst (payload[4])
        pushax  ptr1            ; src (devicespec)
        setax   ptr4            ; length of devicespec. doesn't include the +2
        jsr     _strncpy        ; copy string. this trashes ptr1/2, t1-4

        ; add 0x00 to the end of string as a terminator
        mwa     #_sp_payload+4, ptr1
        adw     ptr1, ptr4      ; ptr1 = (_sp_payload+4) + string length. i.e. location of 0x00 terminator
        ldy     #$00
        tya
        sta     (ptr1), y

        pusha   _sp_network     ; unit
        lda     #'O'            ; open
        jmp     _sp_control     ; do control, and return its errors directly
        ; implicit rts

