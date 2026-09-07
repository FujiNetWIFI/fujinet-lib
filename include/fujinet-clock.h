/**
 * @brief FujiNet Clock Device Library
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_CLOCK_H
#define FUJINET_CLOCK_H

#include <fujinet-int.h>
#include <fujinet-err.h>
#include <fujinet-bus.h>
#include <fujinet-bus-ezclk.h>
#include <fujinet-commands.h>

/*
 * If additional formats are added, do not change the current order of enums.
 * The numeric values are used as indices into the command table in the
 * implementation. Keep TIMEFORMAT_COUNT in sync with the enum.
 */
typedef enum time_format_t {
    SIMPLE_BINARY,            /* 7 bytes: Y(century), Y(hundreds), M, D, H, M, S */
    PRODOS_BINARY,            /* 4 bytes: ProDOS format */
    APETIME_BINARY,           /* 6 bytes: D, M, Y, H, M, S */
    TZ_ISO_STRING,            /* ISO string with current FN timezone, null terminated */
    UTC_ISO_STRING,           /* ISO string in UTC, null terminated */
    APPLE3_SOS_BINARY,        /* Apple3 SOS string: YYYYMMDD0HHMMSS000, null terminated */
    SIMPLE_BINARY_WITH_HUNDREDTHS /* 8 bytes: Y(century), Y(hundreds), M, D, H, M, S, hundredths (0-99) */
} TimeFormat;

#define TIMEFORMAT_COUNT 7

#define CLK_CMD_SIMPLE_BINARY      APETIMECMD_SETTZ_ALT2
#define CLK_CMD_PRODOS_BINARY      APETIMECMD_GET_PRODOS
#ifdef BUILD_APPLE2
#define CLK_CMD_APETIME_BINARY     APETIMECMD_GET_ATARI
#else
#define CLK_CMD_APETIME_BINARY     APETIMECMD_GETTIME
#endif
#define CLK_CMD_TZ_ISO_STRING      APETIMECMD_GET_ISO_LOCAL
#define CLK_CMD_UTC_ISO_STRING     APETIMECMD_GET_ISO_UTC
#define CLK_CMD_APPLE3_SOS_BINARY  APETIMECMD_GET_SOS
#define CLK_CMD_SIMPLE_BINARY_WITH_HUNDREDTHS APETIMECMD_GET_SIMPLE_HUNDREDTHS

#ifdef BUILD_APPLE2
#include <ctype.h>
#define PLATFORM_TZCMD_ALT   APETIMECMD_SETTZ_ALT
#define PLATFORM_TZCMD_MAIN  APETIMECMD_SETTZ_ALT2
#define CLK_ALTIFYERIZE(cmd) tolower(cmd)
#define PLATFORM_CLK_TIME_CALL(format, alt, buf, len) \
  CLKCALL_RV((alt) ? CLK_ALTIFYERIZE(clk_cmd[format]) : clk_cmd[format], buf, len)
#define PLATFORM_CLK_SET_TZ_CALL(cmd, tz) CLKCALL_D(cmd, tz, strlen(tz) + 1)
#define clock_get_time(time_data, format) clock_get_time_common(time_data, format, true)
#else /* ! BUILD_APPLE_2 */
#define PLATFORM_TZCMD_ALT   APETIMECMD_SETTZ
#define PLATFORM_TZCMD_MAIN  APETIMECMD_SETTZ_ALT
#define PLATFORM_CLK_TIME_CALL(format, alt, buf, len) \
  CLKCALL_A1_RV(clk_cmd[format], (alt) ? 1 : 0, buf, len)
#define PLATFORM_CLK_SET_TZ_CALL(cmd, tz) CLKCALL_B12_D(cmd, strlen(tz), tz, strlen(tz))
#define clock_get_time(time_data, format) clock_get_time_common(time_data, format, false)
#endif /* BUILD_APPLE2 */

/**
 * @brief  Set the FN clock's system timezone
 * @param  tz the timezone string to apply
 * @return fujinet status/error code (See FN_ERR_* values)
 */
uint8_t clock_set_tz(const char *tz);

/**
 * @brief  Get the FN clock's system timezone
 * @param  tz pointer to the receiving timezone buffer
 * @return fujinet status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_tz(char *tz);

/**
 * @brief  Get the current time in the format specified using the FN system timezone.
 * @param  time_data pointer to buffer for the response. For string formats the buffer is null terminated.
 * @param  format a TimeFormat value to specify how the data should be returned.
 * @return fujinet status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_time_common(uint8_t *time_data, TimeFormat format, bool alt);

/**
 * @brief  Get the current time in the format specified for the given timezone without affecting the system timezone.
 * @param  time_data pointer to buffer for the response. For string formats the buffer is null terminated.
 * @param  tz pointer to the timezone to use for this call only.
 * @param  format a TimeFormat value to specify how the data should be returned.
 * @return fujinet status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_time_tz(uint8_t *time_data, const char *tz, TimeFormat format);

#endif /* FUJINET_CLOCK_H */
