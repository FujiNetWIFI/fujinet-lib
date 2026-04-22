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
    SIMPLE_BINARY,      /* 7 bytes: Y(century), Y(hundreds), M, D, H, M, S */
    PRODOS_BINARY,      /* 4 bytes: ProDOS format */
    APETIME_BINARY,     /* 6 bytes: D, M, Y, H, M, S */
    TZ_ISO_STRING,      /* ISO string with current FN timezone, null terminated */
    UTC_ISO_STRING,     /* ISO string in UTC, null terminated */
    APPLE3_SOS_BINARY   /* Apple3 SOS string: YYYYMMDD0HHMMSS000, null terminated */
} TimeFormat;

#define TIMEFORMAT_COUNT 6

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
uint8_t clock_get_time(uint8_t *time_data, TimeFormat format);

/**
 * @brief  Get the current time in the format specified for the given timezone without affecting the system timezone.
 * @param  time_data pointer to buffer for the response. For string formats the buffer is null terminated.
 * @param  tz pointer to the timezone to use for this call only.
 * @param  format a TimeFormat value to specify how the data should be returned.
 * @return fujinet status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_time_tz(uint8_t *time_data, const char *tz, TimeFormat format);

#endif /* FUJINET_CLOCK_H */
