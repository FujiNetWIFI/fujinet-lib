#include <stdint.h>

#define PORT_INIT() port_init(1, 16);
#define PORT_TICKS_PER_SECOND 1000

extern void __CALLEE__ port_init(uint8_t ctc_divisor, uint8_t sio_divisor);

// return data if it arrives before timeout or -1 if timeout expires
extern int __FASTCALL__ port_getc_timeout(uint16_t timeout);

// reads and decodes SLIP into two buffers
// returns length of data received, if timeout expires returns all data received until then
// timeout resets when a character is received
extern uint16_t __CALLEE__ port_getbuf_slip_dual(void *hdr_buf, uint16_t hdr_len,
                                                 void *data_buf, uint16_t data_len,
                                                 uint16_t timeout);

// writes character to port
extern void __FASTCALL__ port_putc(uint8_t c);

// writes data to port handling SLIP escapes, returns number of bytes written
extern uint16_t __CALLEE__ port_putbuf_slip(const void *buf, uint16_t len);

#define VDP_IS_PAL (((unsigned char *) 0x002b) & 0x80)
