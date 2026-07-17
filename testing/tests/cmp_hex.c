#include "cmp_hex.h"
#ifdef _CMOC_VERSION_
#include <coco.h>
#else
#include <stdio.h>
#include <stdint.h>
#endif /* _CMOC_VERSION_ */

#define COLUMNS 16

/* =========================================================================
 * Helper: print two arrays as hex and flag differences
 * ========================================================================= */

void print_hex_diff(const char *label,
                    const uint8_t *print_buf, uint8_t print_len,
                    const uint8_t *cmp_buf, uint8_t cmp_len)
{
  int outer, inner, idx;
  uint8_t c;
  bool is_diff, in_diff_hex;


  for (outer = 0; outer < print_len; outer += COLUMNS) {
    printf(" [%s] ", label);

    in_diff_hex = false;

    for (inner = 0; inner < COLUMNS; inner++) {
      idx = outer + inner;

      if (idx < print_len) {
        c = print_buf[idx];
        is_diff = (idx >= cmp_len || c != cmp_buf[idx]);

        if (is_diff && !in_diff_hex) {
          printf("[");
          in_diff_hex = true;
        }
        else if (!is_diff && in_diff_hex) {
          printf("]");
          in_diff_hex = false;
        }
        else
          printf(" ");

        printf("%02x", c);
      }
      else {
        if (in_diff_hex) {
          printf("]  ");
          in_diff_hex = false;
        }
        else
          printf("   ");
      }
    }

    printf("%c|", in_diff_hex ? ']' : ' ');

    for (inner = 0; inner < COLUMNS && (outer + inner) < print_len; inner++) {
      c = print_buf[inner + outer];
      printf("%c", c >= ' ' && c <= 0x7f ? c : '.');
    }

    printf("|\n");
  }

  return;
}

void cmp_hex(const char *label1, const uint8_t *buf1, uint8_t len1,
             const char *label2, const uint8_t *buf2, uint8_t len2)
{
  print_hex_diff(label1, buf1, len1, buf2, len2);
  print_hex_diff(label2, buf2, len2, buf1, len1);
  return;
}
