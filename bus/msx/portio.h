#include <stdint.h>

extern int __FASTCALL__ port_getc();
extern int __FASTCALL__ port_getc_timeout(uint16_t timeout);
extern uint16_t __CALLEE__ port_getbuf(void *buf, uint16_t len, uint16_t timeout);

extern int __FASTCALL__ port_putc(uint8_t c);
extern uint16_t __CALLEE__ port_putbuf(void *buf, uint16_t len);

#define VDP_IS_PAL (((unsigned char *) 0x002b) & 0x80)
