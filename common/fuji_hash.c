#include "fujinet-fuji.h"
#ifndef _CMOC_VERSION_
#include <string.h>
#endif /* _CMOC_VERSION_ */

static uint8_t hash_buf[256];

bool fuji_hash_data(hash_alg_t hash_type, uint8_t *input, uint16_t length,
                    bool as_hex, uint8_t *output)
{
  uint8_t buf[1];


  if (!fuji_hash_clear())
    return false;

  if (!fuji_hash_input(input, length))
    return false;

  if (!fuji_hash_compute((uint8_t) hash_type))
    return false;

  fuji_hash_length(as_hex, buf, 1);
  
  // conveniently, as_hex is 1 or 0 when used as uint8_t

  // hash_len is only really required for systems that ask for fixed
  // block sizes of data from the FujiNet, e.g. atari.

  if (!fuji_hash_output(as_hex, output, buf[0]))
    return false;

  return true;
}

bool fuji_hash_calculate(hash_alg_t hash_type, bool as_hex, bool discard_data, uint8_t *output)
{
  bool is_success;
  uint8_t buf[1];


  is_success = discard_data ? fuji_hash_compute((uint8_t) hash_type)
    : fuji_hash_compute_no_clear((uint8_t) hash_type);
  if (!is_success)
    return false;

  fuji_hash_length(as_hex, buf, 1);

  // conveniently, as_hex is 1 or 0 when used as uint8_t
  // hash_len is only really required for systems that ask for fixed block sizes of data from the FujiNet, e.g. atari.
  if (!fuji_hash_output(as_hex, output, buf[0]))
    return false;

  if (discard_data)
    fuji_hash_clear();

  return is_success;
}

bool fuji_hash_input(const void *data, uint16_t len)
{
  uint16_t count;
  const uint8_t *ptr;


  ptr = (const uint8_t *) data;
  while (len) {
    count = len <= sizeof(hash_buf) - 2 ? len : sizeof(hash_buf) - 2;
    memcpy(&hash_buf[2], ptr, count);
    hash_buf[0] = U16_LSB(count);
    hash_buf[1] = U16_MSB(count);
    if (!FUJICALL_D(FUJICMD_HASH_INPUT, hash_buf, count + 2))
      return false;
    len -= count;
    ptr += count;
  }

  return true;
}
