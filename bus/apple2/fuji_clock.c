#include "fujinet-clock.h"

#include <string.h>
#include <ctype.h>

#define CLK_ALTIFYERIZE(cmd) tolower(cmd)

static const uint8_t clk_reply_len[TIMEFORMAT_COUNT] = { 7, 4, 6, 25, 25, 19 };

extern const uint8_t *clk_cmd;


static uint8_t clock_set_alternate_tz(const char *tz)
{
  return CLKCALL_D(APETIMECMD_SETTZ_ALT, tz, strlen(tz) + 1) ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

uint8_t clock_set_tz(const char *tz)
{
  return CLKCALL_D(APETIMECMD_SETTZ_ALT2, tz, strlen(tz) + 1) ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

uint8_t clock_get_tz(char *tz)
{
  return CLKCALL_RV(APETIMECMD_GET_GENERAL, tz, 64) ? FN_ERR_OK : FN_ERR_IO_ERROR;
}

uint8_t clock_get_time(uint8_t *time_data, TimeFormat format)
{
  if ((uint8_t) format >= TIMEFORMAT_COUNT)
    return FN_ERR_BAD_CMD;
  return CLKCALL_RV(clk_cmd[format], time_data, clk_reply_len[format])
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
  return CLKCALL_RV(CLK_ALTIFYERIZE(clk_cmd[format]), time_data, clk_reply_len[format])
         ? FN_ERR_OK : FN_ERR_IO_ERROR;
}
