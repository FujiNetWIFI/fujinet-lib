// 8255 PPI routines for talking to the FujiNet on the Heathkit H89.
// Adapted from Jeff Tranter's bring-up code in fujinet-bringup/iotest/src/h89/portio.c

#include "portio.h"
#include <arch/z80.h>
#include <stdlib.h>

#define IOPORT 0xD0
#define PORTA IOPORT
#define PORTB IOPORT+1
#define PORTC IOPORT+2
#define PCTRL IOPORT+3

#define PORT_C_EN   2
#define PORT_C_INTR 3
#define PORT_C_STB  4
#define PORT_C_IBF  5
#define PORT_C_ACK  6
#define PORT_C_OBF  7

#define OUTBUF_FULL (1 << PORT_C_OBF) // i8255 wants to send, /OBF, output active low
#define OUTBUF_ACK  (1 << PORT_C_ACK) // ESP32 received the byte, /ACK, input active low
#define INBUF_FULL  (1 << PORT_C_IBF) // i8255 received the byte, IBF, output active high
#define INBUF_GET   (1 << PORT_C_STB) // ESP32 wants to send, /STB, input active low

#define I8255_G2_PORTC_INPUT 0x01
#define I8255_G2_PORTB_INPUT 0x02
#define I8255_G2_MODE_1      0x04
#define I8255_G1_PORTC_INPUT 0x08
#define I8255_G1_PORTA_INPUT 0x10
#define I8255_G1_MODE_1      0x20
#define I8255_G1_MODE_2      0x40
#define I8255_MODE_ACTIVE    0x80

// Jiffy Counter
#ifdef __HDOS__
#define TIKCNT 0x201B    // H89 Jiffy Counter under HDOS.
#else
#define TIKCNT 0x000B    // H89 Jiffy Counter under CP/M.
#endif

void port_init(void)
{
  // Port A in mode 2, port B an input. The card's ESP32 doesn't see the
  // mode word (its EN isn't wired to PC2), so every program can send it.
  z80_outp(PCTRL, I8255_MODE_ACTIVE | I8255_G1_MODE_2 | I8255_G2_PORTB_INPUT);
  z80_inp(PORTA); // Flush input

  return;
}

static int port_getc(void)
{
  int b = -1;

  if (z80_inp(PORTC) & INBUF_FULL)
      b = z80_inp(PORTA);

  return b;
}

// With no card in the slot port C floats high, IBF never clears and port A
// reads FF, so this has to stop on a count rather than on an empty port.
#define DRAIN_MAX 1024

void port_drain(void)
{
    uint16_t n;

    for (n = 0; n < DRAIN_MAX && port_getc() > -1; n++)
        ;
}

int port_getc_timeout(uint16_t timeout)
{
    int b=-1;
    uint16_t start;


    // Store start time so we can calculate delta even when TIKCNT overflows
    start = wpeek(TIKCNT);

    while ((uint16_t) (wpeek(TIKCNT) - start) < timeout)
    {
        b = port_getc();
        if (b > -1)
            return b;
    }
    return b;
}

// Port A runs straight to the ESP32, and the 82C55 drives it only while the
// ESP32 holds /ACK low, so there is no direction to set. Each byte waits for
// the one before it to be taken.
void port_putc(uint8_t c)
{
    while (!(z80_inp(PORTC) & OUTBUF_FULL)); // Wait for ready to handle byte

    z80_outp(PORTA,c);

    return;
}
