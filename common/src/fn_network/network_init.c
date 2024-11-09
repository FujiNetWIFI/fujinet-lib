#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <stdint.h>
#include <ctype.h>
#endif /* _CMOC_VERSION_ */
#include "../../fujinet-network.h"

#ifdef __APPLE2__
#include "fujinet-bus-apple2.h"
#include "sp.h"
#endif

uint8_t network_init(void)
{
  int8_t err = 0;

#ifdef __APPLE2__
  err = sp_init();
  if (err == 0) {
    return FN_ERR_NO_DEVICE;
  } else if (err > 0) {
    // The device was initialised correctly, and we found a network device
    return FN_ERR_OK;
  }
#endif



  return err;
}
