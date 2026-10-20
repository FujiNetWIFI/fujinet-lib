#include <stdint.h>

#define PORT_INIT() port_init()
#define PORT_TICKS_PER_SECOND 500 // H89 clock interrupt is every 2ms

extern void port_init(void);

// return data if it arrives before timeout or -1 if timeout expires
extern int port_getc_timeout(uint16_t timeout);

// reads and decodes SLIP into two buffers
// returns length of data received, if timeout expires returns all data received until then
// timeout resets when a character is received
extern uint16_t port_getbuf_slip_dual(void *hdr_buf, uint16_t hdr_len,
                                      void *data_buf, uint16_t data_len,
                                      uint16_t timeout);

// discards whatever input is waiting (bounded)
extern void port_drain(void);

// writes character to port
extern void port_putc(uint8_t c);

// writes data to port handling SLIP escapes, returns number of bytes written
extern uint16_t port_putbuf_slip(const void *buf, uint16_t len);
