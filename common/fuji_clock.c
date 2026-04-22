#include "fujinet-clock.h"

static const uint8_t _clk_cmd[TIMEFORMAT_COUNT] = {
  CLK_CMD_SIMPLE_BINARY,
  CLK_CMD_PRODOS_BINARY,
  CLK_CMD_APETIME_BINARY,
  CLK_CMD_TZ_ISO_STRING,
  CLK_CMD_UTC_ISO_STRING,
  CLK_CMD_APPLE3_SOS_BINARY
};

const uint8_t *clk_cmd = _clk_cmd;

#ifndef BUILD_APPLE2

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <string.h>
#endif

static const uint8_t clk_reply_len[TIMEFORMAT_COUNT] = { 7, 4, 6, 25, 25, 19 };

static uint8_t clock_set_alternate_tz(const char *tz)
{
  size_t len = strlen(tz) + 1;
  return CLKCALL_B12_D(APETIMECMD_SETTZ, len, tz, len) ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

uint8_t clock_set_tz(const char *tz)
{
  size_t len = strlen(tz) + 1;
  return CLKCALL_B12_D(APETIMECMD_SETTZ_ALT, len, tz, len) ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

uint8_t clock_get_tz(char *tz)
{
  uint8_t len;
  if (!CLKCALL_RV(APETIMECMD_GETTZ_LEN, &len, 1))
    return FN_ERR_IO_ERROR;
  return CLKCALL_RV(APETIMECMD_GET_GENERAL, tz, len) ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

uint8_t clock_get_time(uint8_t *time_data, TimeFormat format)
{
  if ((uint8_t) format >= TIMEFORMAT_COUNT)
    return FN_ERR_BAD_CMD;
  return CLKCALL_A1_RV(clk_cmd[format], 0, time_data, clk_reply_len[format])
         ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

uint8_t clock_get_time_tz(uint8_t *time_data, const char *tz, TimeFormat format)
{
  uint8_t rc;
  if ((uint8_t) format >= TIMEFORMAT_COUNT)
    return FN_ERR_BAD_CMD;
  rc = clock_set_alternate_tz(tz);
  if (rc != FN_ERR_OK)
    return rc;
  return CLKCALL_A1_RV(clk_cmd[format], 1, time_data, clk_reply_len[format])
         ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

#endif /* !BUILD_APPLE2 */
