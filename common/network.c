#include "fujinet-network.h"
#include "fujinet-bus.h"
#include "fujinet-commands.h"
#include "fujinet-const.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
  uint16_t avail;
  uint8_t status;
  uint8_t error;
} NetworkStatus;

NetworkStatus nw_status;

uint8_t network_init()
{
  return FN_ERR_OK;
}

uint8_t network_unit(const char *devicespec)
{
  if (devicespec[1] == ':')
    return 1;

  if (devicespec[2] == ':')
    return devicespec[1] - '0';

  return 1;
}

uint8_t network_open(const char *devicespec, uint8_t mode, uint8_t trans)
{
  return !NETCALL_A1_A2_D(FUJICMD_OPEN, network_unit(devicespec),
			  mode, trans, devicespec, MAX_FILENAME_LEN);
}

uint8_t network_close(const char *devicespec)
{
  return !NETCALL(FUJICMD_CLOSE, network_unit(devicespec));
}

int16_t network_read_nb(const char *devicespec, uint8_t *buf, uint16_t len)
{
  return fuji_bus_read(FUJI_DEVICEID_NETWORK, network_unit(devicespec), buf, len);
}

int16_t network_read(const char *devicespec, uint8_t *buf, uint16_t len)
{
  int16_t rlen, total;


  for (total = 0; len; total += rlen, len -= rlen) {
    rlen = network_read_nb(devicespec, &buf[total], len);
    if (rlen < 0)
      break;
  }

  return total;
}

uint8_t network_write(const char *devicespec, const uint8_t *buf, uint16_t len)
{
  uint8_t nw_unit;
  uint16_t wlen;


  nw_unit = network_unit(devicespec);
  while (len) {
    wlen = fuji_bus_write(FUJI_DEVICEID_NETWORK, nw_unit, buf, len);
    if (len && !wlen)
      return FN_ERR_IO_ERROR;
    len -= wlen;
    buf += wlen;
  }

  return FN_ERR_OK;
}

uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char *devicespec, ...);

uint8_t network_status(const char *devicespec, uint16_t *avail, uint8_t *status, uint8_t *err)
{
  if (!NETCALL_RV(FUJICMD_STATUS, network_unit(devicespec), &nw_status, sizeof(nw_status)))
    return FN_ERR_IO_ERROR;

  *avail = nw_status.avail;
  *status = nw_status.status;
  *err = nw_status.error;

  return FN_ERR_OK;
}
