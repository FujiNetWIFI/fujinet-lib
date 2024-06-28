#ifdef _CMOC_VERSION_
#include <cmoc.h>
#include <coco.h>
#else
#include <stdbool.h>
#include <stdint.h>
#endif /* CMOC_VERSION */

#include "fujinet-fuji.h"

uint16_t fuji_hash_size(hash_alg_t hash_type, bool is_hex)
{
    uint16_t len = 0; // acts as default if hashing algorithm is unknown. Allow for a full word in case we extend to longer hash algorithms
    switch(hash_type)
    {
        case MD5:
            len = 16;
            break;
        case SHA1:
            len = 20;
            break;
        case SHA256:
            len = 32;
            break;
        case SHA512:
            len = 64;
            break;
    }

    // double the value if it's a hex string instead of plain bytes.
    if (is_hex) len <<= 1;

    return len;
}
