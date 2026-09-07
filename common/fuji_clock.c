#include <fujinet-clock.h>

#ifdef _CMOC_VERSION_
#include <cmoc.h>
#else
#include <string.h>
#endif

#define platform_clk_get_tz_len(len_out) \
  { if (!CLKCALL_RV(APETIMECMD_GETTZ_LEN, len_out, 1)) return FN_ERR_IO_ERROR; }

const uint8_t clk_cmd[TIMEFORMAT_COUNT] = {
  CLK_CMD_SIMPLE_BINARY,
  CLK_CMD_PRODOS_BINARY,
  CLK_CMD_APETIME_BINARY,
  CLK_CMD_TZ_ISO_STRING,
  CLK_CMD_UTC_ISO_STRING,
  CLK_CMD_APPLE3_SOS_BINARY,
  CLK_CMD_SIMPLE_BINARY_WITH_HUNDREDTHS
};

const uint8_t clk_reply_len[TIMEFORMAT_COUNT] = { 7, 4, 6, 25, 25, 19, 8 };

static uint8_t clk_result(bool ok)
{
  return ok ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

uint8_t clock_set_tz(const char *tz)
{
  return clk_result(PLATFORM_CLK_SET_TZ_CALL(PLATFORM_TZCMD_MAIN, tz));
}

uint8_t clock_get_tz(char *tz)
{
  uint8_t len;

  platform_clk_get_tz_len(&len);
  return clk_result(CLKCALL_RV(APETIMECMD_GET_GENERAL, tz, len));
}

uint8_t clock_get_time_common(uint8_t *time_data, TimeFormat format, bool alt)
{
  bool success;

  if ((uint8_t) format >= TIMEFORMAT_COUNT)
    return FN_ERR_BAD_CMD;

  success = PLATFORM_CLK_TIME_CALL(format, alt, time_data, clk_reply_len[format]);
  if (success)
    time_data[clk_reply_len[format]] = 0;
  return clk_result(success);
}

uint8_t clock_get_time_tz(uint8_t *time_data, const char *tz, TimeFormat format)
{
  if ((uint8_t) format >= TIMEFORMAT_COUNT)
    return FN_ERR_BAD_CMD;

  if (!PLATFORM_CLK_SET_TZ_CALL(PLATFORM_TZCMD_ALT, tz))
    return FN_ERR_IO_ERROR;
  return clock_get_time_common(time_data, format, true);
}
