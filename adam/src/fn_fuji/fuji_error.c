#include <stdbool.h>
#include <stdint.h>
#include <eos.h>
#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include "fujinet-fuji-adam.h"

bool fuji_error()
{
  // Gonna guess here.
  DCB *dcb;

  return eos_request_device_status(FUJINET_DEVICE_ID,dcb) != ADAMNET_OK;
}
