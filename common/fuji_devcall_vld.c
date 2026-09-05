#include <fujinet-bus.h>

// Sends a variable length packet with data on systems that have
// variable length packets, otherwise uses the aux1/aux2 bytes to
// encode the length of the data using native byte endianness.
bool fuji_devcall_vld(uint8_t device, uint8_t cmd, const void *data, uint16_t data_len)
{
  bool success;

#if FUJI_VARIABLE_LEN_PACKETS
  if (data_len)
    success = DEVCALL_D(device, cmd, data, data_len);
  else
    success = DEVCALL(device, cmd);
#else /* ! FUJI_VARIABLE_LEN_PACKETS */
  if (data_len)
    success = DEVCALL_B12_D(device, cmd, data_len, data, data_len);
  else
    success = DEVCALL_B12(device, cmd, data_len);
#endif /* FUJI_VARIABLE_LEN_PACKETS */

  return success;
}
