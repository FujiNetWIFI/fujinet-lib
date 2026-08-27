#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"

// Not implemented for AdamNet. See fuji_qrcode_input.c.
bool fuji_qrcode_encode(uint8_t version, uint8_t ecc, bool shorten)
{
  return false;
}
