#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"

// Not implemented for AdamNet. See fuji_qrcode_input.c.
bool fuji_qrcode_length(uint8_t output_mode, unsigned long *len)
{
  return false;
}
