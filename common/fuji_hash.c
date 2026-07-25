#include <fujinet-fuji.h>
#ifndef _CMOC_VERSION_
#include <string.h>
#endif /* _CMOC_VERSION_ */

bool fuji_hash_data(hash_alg_t hash_type, uint8_t *input, uint16_t length,
                    bool as_hex, uint8_t *output)
{
  uint8_t c;


  if (!fuji_hash_clear())
    return false;

  if (!fuji_hash_input(input, length))
    return false;

  if (!fuji_hash_compute((uint8_t) hash_type))
    return false;

  fuji_hash_length(as_hex, &c, 1);

  // conveniently, as_hex is 1 or 0 when used as uint8_t

  // hash_len is only really required for systems that ask for fixed
  // block sizes of data from the FujiNet, e.g. atari.

  if (!fuji_hash_output(as_hex, output, c))
    return false;

  return true;
}
