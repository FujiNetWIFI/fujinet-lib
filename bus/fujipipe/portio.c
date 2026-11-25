// The pipe of a FujiNet I/O card (portio.h), polled. Its STATUS carries a
// signature, AVAIL for a byte to read and TXFULL while the card's queue is full.

#include "portio.h"
#include <arch/z80.h>

#define PIPE_GETC    FUJIPIPE_BASE
#define PIPE_STATUS  (FUJIPIPE_BASE + 1)
#define PIPE_PUTC    (FUJIPIPE_BASE + 2)
#define PIPE_CONTROL (FUJIPIPE_BASE + 3)
#define PIPE_TICK    (FUJIPIPE_BASE + 3)

#define ST_AVAIL  0x80
#define ST_TXFULL 0x01
#define CTL_FLUSH 0x08

// TXFULL is advisory: after this many looks the byte goes anyway.
#define TXFULL_WAIT 256

static uint8_t tick_last;
static uint16_t tick_total;

uint16_t port_ticks(void)
{
  uint8_t now = z80_inp(PIPE_TICK);

  tick_total += (uint8_t) (now - tick_last);
  tick_last = now;
  return tick_total;
}

void port_init(void)
{
  tick_last = z80_inp(PIPE_TICK);
  z80_outp(PIPE_CONTROL, CTL_FLUSH);
}

void port_drain(void)
{
  z80_outp(PIPE_CONTROL, CTL_FLUSH);
}

int port_getc_timeout(uint16_t timeout)
{
  uint16_t start = port_ticks();

  do {
    if (z80_inp(PIPE_STATUS) & ST_AVAIL)
      return z80_inp(PIPE_GETC);
  } while ((uint16_t) (port_ticks() - start) < timeout);
  return -1;
}

void port_putc(uint8_t value)
{
  uint16_t look;

  for (look = 0; look < TXFULL_WAIT && (z80_inp(PIPE_STATUS) & ST_TXFULL); look++)
    ;
  z80_outp(PIPE_PUTC, value);
}
