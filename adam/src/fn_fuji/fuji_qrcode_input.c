#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"

// Not implemented for AdamNet.
//
// The QR commands need parameters ahead of their payload, and the AdamNet port
// has no existing example of encoding those -- its base64/hash functions send a
// bare command byte followed by data. Returning false is deliberate: a stub
// returning true would have callers render whatever happened to be in their
// buffer as a QR code.
bool fuji_qrcode_input(char *s, uint16_t len)
{
  return false;
}
