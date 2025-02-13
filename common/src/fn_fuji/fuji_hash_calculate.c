#if defined(_CMOC_VERSION_)

#include <cmoc.h>
#include <coco.h>

#else

#include <stdbool.h>
#include <stdint.h>

#endif // _CMOC_VERSION_

#include "fujinet-fuji.h"

bool fuji_hash_calculate(hash_alg_t hash_type, bool as_hex, bool discard_data, uint8_t *output)
{
	uint8_t hash_len;
	bool is_success;

	hash_len = fuji_hash_size(hash_type, as_hex);

	is_success = discard_data ? fuji_hash_compute(hash_type) : fuji_hash_compute_no_clear(hash_type);

	// conveniently, as_hex is 1 or 0 when used as uint8_t
	// hash_len is only really required for systems that ask for fixed block sizes of data from the FujiNet, e.g. atari.
	if (is_success) is_success = fuji_hash_output((uint8_t) as_hex, (char *) output, hash_len);

	if (discard_data) fuji_hash_clear();

	return is_success;
}

