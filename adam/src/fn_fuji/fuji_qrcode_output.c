#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"

// Not implemented for AdamNet. See fuji_qrcode_input.c.
bool fuji_qrcode_output(char *s, uint16_t len)
{
  return false;
}
