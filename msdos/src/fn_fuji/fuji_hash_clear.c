#include <stdbool.h>
#include <stdint.h>
#include "fujinet-fuji.h"
#include "fujinet-fuji-msdos.h"

bool fuji_hash_clear(void)
{
  return int_f5(0x70,0xC2,0x00,0x00) == 'C';
}
