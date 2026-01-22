#ifndef NETWORK_UNIT_STATUS_DEFAULT_H
#define NETWORK_UNIT_STATUS_DEFAULT_H

#include <fujinet-int.h>
#include <fujinet-err.h>

typedef struct {
  uint16_t avail;
  uint8_t status;
  uint8_t errcode;
} NetworkStatus;

extern FN_ERR network_unit_status(uint8_t unit, NetworkStatus *nw_status);

#endif /* NETWORK_UNIT_STATUS_DEFAULT_H */
