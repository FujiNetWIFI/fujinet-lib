#include "print_hex.h"
#ifdef _CMOC_VERSION_
#include <coco.h>
#else
#include <stdio.h>
#include <stdint.h>
#endif /* _CMOC_VERSION_ */

#define COLUMNS 16

/* =========================================================================
 * Helper: print a byte array as hex
 * ========================================================================= */
void print_hex(const char *label, const uint8_t *buf, size_t len)
{
  int outer, inner;
  uint8_t c;


  for (outer = 0; outer < len; outer += COLUMNS) {
    printf("  [%s] ", label);
    for (inner = 0; inner < COLUMNS; inner++) {
      if (inner + outer < len) {
	c = buf[inner + outer];
	printf("%02x ", c);
      }
      else
	printf("   ");
    }
    printf(" |");
    for (inner = 0; inner < COLUMNS && inner + outer < len; inner++) {
      c = buf[inner + outer];
      printf("%c", c >= ' ' && c <= 0x7f ? c : '.');
    }
    printf("|\n");
  }

  return;
}
