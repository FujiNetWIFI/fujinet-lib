#include "network_unit_status.h"

#include <fujinet-network.h>
#include <fujinet-bus.h>
#include <fujinet-commands.h>
#include <fujinet-const.h>
#include <fujinet-err.h>

NetworkStatus nw_status;

FN_ERR network_init()
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

FN_ERR network_open(const char *devicespec, uint8_t mode, uint8_t trans)
{
  bool success;
  uint8_t nw_unit = network_unit(devicespec);


  success = NETCALL_A1_A2_D(FUJICMD_OPEN, nw_unit,
			    mode, trans, devicespec, MAX_FILENAME_LEN);
  if (!success)
    return FN_ERR_IO_ERROR;

#if 0
  // FIXME - if this is a POST open then calling status will try do do the POST with no data!
  if (!network_unit_status(network_unit(devicespec), &nw_status))
    return FN_ERR_IO_ERROR;

  /* We haven't even read the file yet, it's not EOF */
  if (nw_status.errcode == NETWORK_ERROR_END_OF_FILE)
    nw_status.errcode = NETWORK_SUCCESS;

  if (nw_status.errcode > NETWORK_SUCCESS && !nw_status.avail)
    return nw_status.errcode;
#endif

  return FN_ERR_OK;
}

FN_ERR network_close(const char *devicespec)
{
  return !NETCALL(FUJICMD_CLOSE, network_unit(devicespec));
}

int16_t network_read_nb(const char *devicespec, void *buf, uint16_t len)
{
  uint8_t nw_unit;
  FN_ERR err;


  nw_unit = network_unit(devicespec);

  do {
    // Check how many bytes are available
    err = network_unit_status(nw_unit, &nw_status);
    if (err != FN_ERR_OK)
      return -err;

    if (nw_status.errcode > NETWORK_SUCCESS && !nw_status.avail) {
      fn_device_error = nw_status.errcode;
      return 0;
    }
  } while (!nw_status.avail);

  if (len > nw_status.avail)
    len = nw_status.avail;

  return fuji_bus_read(FUJI_DEVICEID_NETWORK + nw_unit - 1, buf, len);
}

int16_t network_read(const char *devicespec, void *buf, uint16_t len)
{
  int16_t total;
  uint8_t *ubuf = (uint8_t *) buf;
  int16_t rlen = 0;


  for (total = 0; len; total += rlen, len -= rlen) {
    rlen = network_read_nb(devicespec, &ubuf[total], len);
    if (rlen < 1)
      break;
  }

  if (!total && rlen < 0)
    return rlen;
  return total;
}

FN_ERR network_write(const char *devicespec, const void *buf, uint16_t len)
{
  uint8_t nw_unit;
  uint16_t wlen;
  uint8_t *ubuf = (uint8_t *) buf;


  nw_unit = network_unit(devicespec);
  while (len) {
    wlen = fuji_bus_write(FUJI_DEVICEID_NETWORK + nw_unit - 1, ubuf, len);
    if (len && !wlen)
      return FN_ERR_IO_ERROR;
    len -= wlen;
    ubuf += wlen;
  }

  return FN_ERR_OK;
}

FN_ERR network_status(const char *devicespec, uint16_t *avail, uint8_t *status, uint8_t *err)
{
  FN_ERR stat_err = network_unit_status(network_unit(devicespec), &nw_status);

  if (stat_err != FN_ERR_OK)
    return stat_err;

  *avail = nw_status.avail;
  *status = nw_status.status;
  *err = nw_status.errcode;

  return FN_ERR_OK;
}

FN_ERR network_accept(const char *devicespec)
{
  uint8_t nw_unit;
  bool success;


  nw_unit = network_unit(devicespec);
  success = NETCALL(FUJICMD_ACCEPT, nw_unit);
  if (!success)
    return FN_ERR_IO_ERROR;
  return FN_ERR_OK;
}
