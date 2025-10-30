#include <stdint.h>

// returns signed int with data or -1 if no data is available
extern int __FASTCALL__ port_getc();

// return data if it arrives before timeout or -1 if timeout expires
extern int __FASTCALL__ port_getc_timeout(uint16_t timeout);

// reads data until c is seen or timeout occurs, returns c or -1 on timeout
// timeout *does not reset* when a character is received
extern int __FASTCALL__ port_discard_until(uint8_t c, uint16_t timeout);

// returns length of data received, if timeout expires returns all data received until then
// timeout resets when a character is received
extern uint16_t __CALLEE__ port_getbuf(void *buf, uint16_t len, uint16_t timeout);

// reads until c is found, timeout, or maxlen is reached
// returns length of data received, including c
// timeout resets when a character is received
extern uint16_t __CALLEE__ port_get_until(void *buf, uint16_t maxlen, uint8_t c,
                                          uint16_t timeout);

// writes character to port
extern int __FASTCALL__ port_putc(uint8_t c);

// writes data to port, returns number of bytes written
extern uint16_t __CALLEE__ port_putbuf(void *buf, uint16_t len);

#define VDP_IS_PAL (((unsigned char *) 0x002b) & 0x80)
