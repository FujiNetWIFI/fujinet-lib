// SLIP framing on top of the H89 port routines. Same behavior as the
// assembly versions in bus/model2.

#include "portio.h"

enum {
  SLIP_END     = 0xC0,
  SLIP_ESCAPE  = 0xDB,
  SLIP_ESC_END = 0xDC,
  SLIP_ESC_ESC = 0xDD,
};

#define SYNC_MAX 4096

uint16_t port_putbuf_slip(const void *buf, uint16_t len)
{
  uint16_t idx, count;
  const uint8_t *ptr = (const uint8_t *) buf;
  uint8_t c;


  for (idx = count = 0; idx < len; idx++) {
    c = ptr[idx];
    if (c == SLIP_END) {
      port_putc(SLIP_ESCAPE);
      count++;
      c = SLIP_ESC_END;
    }
    else if (c == SLIP_ESCAPE) {
      port_putc(SLIP_ESCAPE);
      count++;
      c = SLIP_ESC_ESC;
    }
    port_putc(c);
    count++;
  }

  return count;
}

uint16_t port_getbuf_slip_dual(void *hdr_buf, uint16_t hdr_len,
                               void *data_buf, uint16_t data_len,
                               uint16_t timeout)
{
  uint8_t *ptr = (uint8_t *) hdr_buf;
  uint16_t remain = hdr_len, total = 0, skipped = 0;
  int c;


  if (!remain) {
    ptr = (uint8_t *) data_buf;
    remain = data_len;
    data_len = 0;
  }
  if (!remain)
    return 0;

  // Sync to frame: discard until SLIP_END. The timeout restarts with each
  // byte, so it alone never ends this when bytes keep coming without an END
  // -- which is what an empty slot does: port C floats high and port A reads
  // FF, as fast as it is asked. Nothing real is that far out of frame.
  do {
    c = port_getc_timeout(timeout);
    if (c < 0 || ++skipped > SYNC_MAX)
      return 0;
  } while (c != SLIP_END);

  // Skip any additional SLIP_END
  do {
    c = port_getc_timeout(timeout);
    if (c < 0)
      return 0;
  } while (c == SLIP_END);

  for (;;) {
    if (c == SLIP_END)
      break;

    if (c == SLIP_ESCAPE) {
      c = port_getc_timeout(timeout);
      if (c < 0)
        break;
      if (c == SLIP_ESC_END)
        c = SLIP_END;
      else if (c == SLIP_ESC_ESC)
        c = SLIP_ESCAPE;
      // Unknown escape -- store as-is
    }

    *ptr++ = c;
    total++;
    if (!--remain) {
      // Current buffer full, switch to data buffer if there is one
      if (!data_len)
        break;
      ptr = (uint8_t *) data_buf;
      remain = data_len;
      data_len = 0;
    }

    c = port_getc_timeout(timeout);
    if (c < 0)
      break;
  }

  return total;
}
