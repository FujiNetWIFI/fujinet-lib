#include <fujinet-fuji.h>

bool fuji_hash_calculate(hash_alg_t hash_type, bool as_hex, bool discard_data, uint8_t *output)
{
  bool is_success;
  uint8_t c;


  is_success = discard_data ? fuji_hash_compute((uint8_t) hash_type)
    : fuji_hash_compute_no_clear((uint8_t) hash_type);
  if (!is_success)
    return false;

  fuji_hash_length(as_hex, &c, 1);

  // conveniently, as_hex is 1 or 0 when used as uint8_t
  // hash_len is only really required for systems that ask for fixed block sizes of data from the FujiNet, e.g. atari.
  if (!fuji_hash_output(as_hex, output, c))
    return false;

  if (discard_data)
    fuji_hash_clear();

  return is_success;
}
