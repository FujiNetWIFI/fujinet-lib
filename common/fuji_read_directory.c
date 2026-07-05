#include <fujinet-fuji.h>

bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
{
  return FUJICALL_A1_A2_RV(FUJICMD_READ_DIR_ENTRY, maxlen, aux2, buffer, maxlen);
}


