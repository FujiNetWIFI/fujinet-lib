// 8255 PPI routines for talking to the FujiNet on the Heathkit H89.
// Adapted from Jeff Tranter's bring-up code in fujinet-bringup/iotest/src/h89/portio.c

#include "portio.h"
#include <arch/z80.h>
#include <stdlib.h>
#include <stdbool.h>

#define IOPORT 0xD0
#define PORTA IOPORT
#define PORTB IOPORT+1
#define PORTC IOPORT+2
#define PCTRL IOPORT+3

#define PORT_C_DIR  0
#define PORT_C_OE   1
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

#define DATA_DIR    0x01
#define DATA_EN     0x02

#define I8255_G2_PORTC_INPUT 0x01
#define I8255_G2_PORTB_INPUT 0x02
#define I8255_G2_MODE_1      0x04
#define I8255_G1_PORTC_INPUT 0x08
#define I8255_G1_PORTA_INPUT 0x10
#define I8255_G1_MODE_1      0x20
#define I8255_G1_MODE_2      0x40
#define I8255_MODE_ACTIVE    0x80

#define OE_ENABLE  0
#define OE_DISABLE 1

// 74LVC245 Direction
enum {
  ESP32_TO_H89 = 0,
  H89_TO_ESP32 = 1,
  NONE_TO_NONE = 2,
};

// Jiffy Counter
#ifdef __HDOS__
#define TIKCNT 0x201B    // H89 Jiffy Counter under HDOS.
#else
#define TIKCNT 0x000B    // H89 Jiffy Counter under CP/M.
#endif

static unsigned char current_dir = NONE_TO_NONE;
static unsigned char out_enabled = 0;

static void i8255_set_level(uint_fast8_t pin, bool level)
{
  pin <<= 1;
  pin |= level;
  z80_outp(PCTRL, pin);
  return;
}

static void port_set_direction(unsigned char dir)
{
    if (dir == current_dir)
       return;

    if (dir == NONE_TO_NONE) {
      i8255_set_level(PORT_C_OE, OE_DISABLE);
      out_enabled = 0;
    }
    else {
      if (!out_enabled) {
        i8255_set_level(PORT_C_OE, OE_ENABLE);
        out_enabled = 1;
      }

      z80_outp(PCTRL,dir);
    }

    current_dir = dir;
}

void port_init(void)
{
  // Port A in mode 2, port B an input. The card's ESP32 doesn't see the
  // mode word (its EN isn't wired to PC2), so every program can send it.
  z80_outp(PCTRL, I8255_MODE_ACTIVE | I8255_G1_MODE_2 | I8255_G2_PORTB_INPUT);
  z80_inp(PORTA); // Flush input

  port_set_direction(ESP32_TO_H89);

  return;
}

static int port_getc(void)
{
  int b = -1;

  if (z80_inp(PORTC) & INBUF_FULL)
  {
      port_set_direction(ESP32_TO_H89);
      b = z80_inp(PORTA);
  }

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

void port_putc(uint8_t c)
{
    port_set_direction(H89_TO_ESP32);

    while (!(z80_inp(PORTC) & OUTBUF_FULL)); // Wait for ready to handle byte

    z80_outp(PORTA,c);

    while (!(z80_inp(PORTC) & OUTBUF_FULL)); // Wait for byte accepted

    port_set_direction(ESP32_TO_H89);

    return;
}
