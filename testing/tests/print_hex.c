#ifdef _CMOC_VERSION_
#include <coco.h>
#else
#include <stdio.h>
#include <stdint.h>
#endif /* _CMOC_VERSION_ */

/* =========================================================================
 * Helper: print a byte array as hex
 * ========================================================================= */
void print_hex(const char *label, const uint8_t *buf, uint8_t len)
{
    uint8_t i;
    printf("  [%s] ", label);
    for (i = 0; i < len; i++) {
        printf("%02X ", buf[i]);
    }
    printf("\n");
}
