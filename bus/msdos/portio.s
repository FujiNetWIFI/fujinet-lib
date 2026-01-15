; 8250 UART Serial I/O Driver for 8088
; Implements serial communication functions for COM1 (port 0x3F8)
;
; Baud Rate Configuration (1.8432 MHz crystal):
;   115200 baud: divisor = 1
;    57600 baud: divisor = 2
;    38400 baud: divisor = 3
;    19200 baud: divisor = 6
;     9600 baud: divisor = 12
;     4800 baud: divisor = 24
;     2400 baud: divisor = 48
;
; Change BAUD_DIVISOR below to set desired baud rate

        .model  small
        .8086

        ; Baud rate divisor - change this to set baud rate
BAUD_DIVISOR    EQU     1               ; 115200 baud

        ; 8250 UART Register Offsets
UART_BASE       EQU     3F8h            ; COM1 base address
UART_RBR        EQU     UART_BASE+0     ; Receiver Buffer Register (read)
UART_THR        EQU     UART_BASE+0     ; Transmitter Holding Register (write)
UART_IER        EQU     UART_BASE+1     ; Interrupt Enable Register
UART_IIR        EQU     UART_BASE+2     ; Interrupt Identification Register
UART_LCR        EQU     UART_BASE+3     ; Line Control Register
UART_MCR        EQU     UART_BASE+4     ; Modem Control Register
UART_LSR        EQU     UART_BASE+5     ; Line Status Register
UART_MSR        EQU     UART_BASE+6     ; Modem Status Register
UART_DLL        EQU     UART_BASE+0     ; Divisor Latch Low (when DLAB=1)
UART_DLH        EQU     UART_BASE+1     ; Divisor Latch High (when DLAB=1)

        ; Line Status Register bits
LSR_DR          EQU     01h             ; Data Ready
LSR_THRE        EQU     20h             ; Transmitter Holding Register Empty

        ; Line Control Register bits
LCR_DLAB        EQU     80h             ; Divisor Latch Access Bit
LCR_8N1         EQU     03h             ; 8 data bits, no parity, 1 stop bit

        ; Modem Control Register bits
MCR_DTR         EQU     01h             ; Data Terminal Ready
MCR_RTS         EQU     02h             ; Request To Send
MCR_OUT2        EQU     08h             ; OUT2 (enables interrupts on PC)

        .code

        PUBLIC  _port_init
        PUBLIC  _port_getc
        PUBLIC  _port_getc_timeout
        PUBLIC  _port_getbuf
        PUBLIC  _port_putc
        PUBLIC  _port_putbuf

;-----------------------------------------------------------------------------
; void port_init(void)
; Initialize the UART for 115200 baud, 8N1
;-----------------------------------------------------------------------------
_port_init      PROC    NEAR
        push    ax
        push    dx

        ; Set DLAB to access divisor latch
        mov     dx, UART_LCR
        mov     al, LCR_DLAB
        out     dx, al

        ; Set baud rate using BAUD_DIVISOR
        mov     dx, UART_DLL
        mov     al, BAUD_DIVISOR AND 0FFh
        out     dx, al
        mov     dx, UART_DLH
        mov     al, (BAUD_DIVISOR SHR 8) AND 0FFh
        out     dx, al

        ; Set line control: 8N1, clear DLAB
        mov     dx, UART_LCR
        mov     al, LCR_8N1
        out     dx, al

        ; Enable DTR, RTS, OUT2
        mov     dx, UART_MCR
        mov     al, MCR_DTR OR MCR_RTS OR MCR_OUT2
        out     dx, al

        ; Disable interrupts
        mov     dx, UART_IER
        xor     al, al
        out     dx, al

        ; Clear any pending data
        mov     dx, UART_RBR
        in      al, dx

        pop     dx
        pop     ax
        ret
_port_init      ENDP

;-----------------------------------------------------------------------------
; int port_getc(void)
; Read one character from the UART if available
; Returns: Character in AX (0-255), or -1 if no data available
;-----------------------------------------------------------------------------
_port_getc      PROC    NEAR
        push    dx

        mov     dx, UART_LSR
        in      al, dx
        test    al, LSR_DR              ; Check if data ready
        jz      getc_no_data

        ; Read the character
        mov     dx, UART_RBR
        in      al, dx
        xor     ah, ah                  ; Zero extend to word
        jmp     getc_done

getc_no_data:
        mov     ax, -1                  ; No data available

getc_done:
        pop     dx
        ret
_port_getc      ENDP

;-----------------------------------------------------------------------------
; int _port_getctimeout(uint16_t timeout)
; Wait for a character with timeout in milliseconds
; Parameters: timeout in stack (milliseconds)
; Returns: Character in AX (0-255), or -1 on timeout
;-----------------------------------------------------------------------------
_port_getc_timeout PROC NEAR
        push    bp
        mov     bp, sp
        push    bx
        push    cx
        push    dx
        push    es

        ; Read starting BIOS tick count (0040:006C)
        mov     ax, 40h
        mov     es, ax
        mov     bx, es:[6Ch]            ; Get current tick count

        ; Convert timeout from ms to ticks (timeout / 55)
        mov     ax, [bp+4]              ; Get timeout parameter in ms
        mov     cx, 55
        xor     dx, dx
        div     cx                      ; AX = timeout in ticks
        mov     cx, ax                  ; CX = timeout in ticks
        add     cx, bx                  ; CX = end tick count

getct_check:
        push    cx
        mov     dx, UART_LSR
        in      al, dx
        test    al, LSR_DR              ; Check if data ready
        pop     cx
        jnz     getct_got_char

        ; Check if timeout expired
        push    cx
        mov     ax, 40h
        mov     es, ax
        mov     ax, es:[6Ch]            ; Get current tick count
        pop     cx
        cmp     ax, cx                  ; Compare current to end time
        jb      getct_check             ; Continue if not expired

        ; Timeout occurred
        mov     ax, -1
        jmp     getct_done

getct_got_char:
        ; Read the character
        mov     dx, UART_RBR
        in      al, dx
        xor     ah, ah                  ; Zero extend to word

getct_done:
        pop     es
        pop     dx
        pop     cx
        pop     bx
        pop     bp
        ret
_port_getc_timeout ENDP

;-----------------------------------------------------------------------------
; uint16_t port_getbuf(void *buf, uint16_t len, uint16_t timeout)
; Read multiple characters into buffer with timeout in milliseconds
; Parameters: buf (pointer), len (word), timeout (word in ms)
; Returns: Number of characters actually read in AX
;-----------------------------------------------------------------------------
_port_getbuf    PROC    NEAR
        push    bp
        mov     bp, sp
        push    bx
        push    cx
        push    dx
        push    di
        push    es

        mov     di, [bp+4]              ; Get buffer pointer

        push    ds
        pop     es                      ; ES = DS for stosb

        xor     cx, cx                  ; Count of chars read

getb_read_loop:
        cmp     cx, [bp+6]              ; Check if we've read requested length
        jae     getb_done

        ; Read starting BIOS tick count for this character
        push    cx
        push    ds
        mov     ax, 40h
        mov     ds, ax
        mov     bx, ds:[6Ch]            ; Get current tick count
        pop     ds

        ; Convert timeout from ms to ticks (timeout / 55)
        mov     ax, [bp+8]              ; Get timeout parameter in ms
        push    dx
        mov     dx, 0
        push    cx
        mov     cx, 55
        div     cx                      ; AX = timeout in ticks
        pop     cx
        pop     dx
        add     ax, bx                  ; AX = end tick count
        mov     bx, ax                  ; BX = end tick count

getb_wait_char:
        push    bx
        mov     dx, UART_LSR
        in      al, dx
        test    al, LSR_DR
        pop     bx
        jnz     getb_got_char

        ; Check if timeout expired
        push    bx
        push    cx
        push    ds
        mov     ax, 40h
        mov     ds, ax
        mov     ax, ds:[6Ch]            ; Get current tick count
        pop     ds
        pop     cx
        pop     bx
        cmp     ax, bx                  ; Compare current to end time
        jb      getb_wait_char          ; Continue if not expired

        pop     cx                      ; Timeout - return what we have
        jmp     getb_done

getb_got_char:
        pop     cx
        mov     dx, UART_RBR
        in      al, dx
        stosb                           ; Store char and increment DI
        inc     cx
        jmp     getb_read_loop

getb_done:
        mov     ax, cx                  ; Return count

        pop     es
        pop     di
        pop     dx
        pop     cx
        pop     bx
        pop     bp
        ret
_port_getbuf    ENDP

;-----------------------------------------------------------------------------
; int port_putc(uint8_t c)
; Send one character to the UART
; Parameters: c (byte) on stack
; Returns: Character sent in AX, or -1 on error
;-----------------------------------------------------------------------------
_port_putc      PROC    NEAR
        push    bp
        mov     bp, sp
        push    dx

putc_wait:
        mov     dx, UART_LSR
        in      al, dx
        test    al, LSR_THRE            ; Check if transmitter ready
        jz      putc_wait

        ; Send the character
        mov     al, [bp+4]              ; Get character parameter
        mov     dx, UART_THR
        out     dx, al

        xor     ah, ah                  ; Return character in AX

        pop     dx
        pop     bp
        ret
_port_putc      ENDP

;-----------------------------------------------------------------------------
; uint16_t port_putbuf(void *buf, uint16_t len)
; Send multiple characters from buffer
; Parameters: buf (pointer), len (word)
; Returns: Number of characters sent in AX
;-----------------------------------------------------------------------------
_port_putbuf    PROC    NEAR
        push    bp
        mov     bp, sp
        push    bx
        push    cx
        push    dx
        push    si
        push    ds

        mov     si, [bp+4]              ; Get buffer pointer
        mov     cx, [bp+6]              ; Get length

        xor     bx, bx                  ; Count of chars sent

putb_send_loop:
        cmp     bx, cx                  ; Check if done
        jae     putb_done

putb_wait:
        mov     dx, UART_LSR
        in      al, dx
        test    al, LSR_THRE            ; Check if transmitter ready
        jz      putb_wait

        ; Send character
        lodsb                           ; Load char from [DS:SI] and increment SI
        mov     dx, UART_THR
        out     dx, al

        inc     bx
        jmp     putb_send_loop

putb_done:
        mov     ax, bx                  ; Return count

        pop     ds
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     bp
        ret
_port_putbuf    ENDP

        END
