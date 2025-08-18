#include "fujinet-network.h"
#include "fujinet-fuji.h"
#include "fujinet-bus.h"
#include "fujinet-commands.h"
#include "fujinet-const.h"
#include "fujinet-err.h"

typedef struct {
  uint16_t avail;
  uint8_t status;
  uint8_t errcode;
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
  bool success;
  uint8_t nw_unit = network_unit(devicespec);


  success = NETCALL_A1_A2_D(FUJICMD_OPEN, nw_unit,
			    mode, trans, devicespec, MAX_FILENAME_LEN);
  if (!success)
    return FN_ERR_IO_ERROR;

  if (!NETCALL_RV(FUJICMD_STATUS, nw_unit, &nw_status, sizeof(nw_status)))
    return FN_ERR_IO_ERROR;

  /* We haven't even read the file yet, it's not EOF */
  if (nw_status.errcode == NETWORK_ERROR_END_OF_FILE)
    nw_status.errcode = NETWORK_SUCCESS;

  if (nw_status.errcode > NETWORK_SUCCESS && !nw_status.avail)
    return nw_status.errcode;

  return FN_ERR_OK;
}

uint8_t network_close(const char *devicespec)
{
  return !NETCALL(FUJICMD_CLOSE, network_unit(devicespec));
}

int16_t network_read_nb(const char *devicespec, void *buf, uint16_t len)
{
  uint8_t nw_unit;


  nw_unit = network_unit(devicespec);

  do {
    // Check how many bytes are available
    if (!NETCALL_RV(FUJICMD_STATUS, nw_unit, &nw_status, sizeof(nw_status)))
      return -FN_ERR_IO_ERROR;

    if (nw_status.errcode > NETWORK_SUCCESS && !nw_status.avail) {
      fn_device_error = nw_status.errcode;
      return 0;
    }
  } while (!nw_status.avail);

  if (len > nw_status.avail)
    len = nw_status.avail;

  return fuji_bus_read(FUJI_DEVICEID_NETWORK, nw_unit, buf, len);
}

int16_t network_read(const char *devicespec, void *buf, uint16_t len)
{
  int16_t rlen, total;
  uint8_t *ubuf = (uint8_t *) buf;


  for (total = 0; len; total += rlen, len -= rlen) {
    rlen = network_read_nb(devicespec, &ubuf[total], len);
    if (rlen < 1)
      break;
  }

  return total;
}

uint8_t network_write(const char *devicespec, const void *buf, uint16_t len)
{
  uint8_t nw_unit;
  uint16_t wlen;
  uint8_t *ubuf = (uint8_t *) buf;


  nw_unit = network_unit(devicespec);
  while (len) {
    wlen = fuji_bus_write(FUJI_DEVICEID_NETWORK, nw_unit, ubuf, len);
    if (len && !wlen)
      return FN_ERR_IO_ERROR;
    len -= wlen;
    ubuf += wlen;
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
  *err = nw_status.errcode;

  return FN_ERR_OK;
}
