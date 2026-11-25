// The FujiNet I/O cards' FujiBus pipe: four ports at a base, +0 GETC, +1 STATUS,
// +2 PUTC, +3 CONTROL (write) or TICK (read, a free-running 1 kHz count). The
// platform sets FUJIPIPE_BASE to its card's base.
#include <stdint.h>

#define PORT_INIT() port_init()
#define PORT_TICKS_PER_SECOND 1000

#ifndef FUJIPIPE_BASE
#error "Platform did not set FUJIPIPE_BASE"
#endif

extern void port_init(void);

// return data if it arrives before timeout or -1 if timeout expires
extern int port_getc_timeout(uint16_t timeout);

// reads and decodes SLIP into two buffers
// returns length of data received, if timeout expires returns all data received until then
// timeout resets when a character is received
extern uint16_t port_getbuf_slip_dual(void *hdr_buf, uint16_t hdr_len,
                                      void *data_buf, uint16_t data_len,
                                      uint16_t timeout);

// discards whatever input is waiting
extern void port_drain(void);

// writes character to port
extern void port_putc(uint8_t value);

// writes data to port handling SLIP escapes, returns number of bytes written
extern uint16_t port_putbuf_slip(const void *buf, uint16_t len);

// The card's millisecond count, widened from TICK's 8 bits; call it at least
// every quarter second or a wrap is missed.
extern uint16_t port_ticks(void);
