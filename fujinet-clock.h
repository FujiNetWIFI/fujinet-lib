
/**
 * @brief FujiNet Clock Device Library
 * @license gpl v. 3, see LICENSE for details.
 */

#ifndef FUJINET_CLOCK_H
#define FUJINET_CLOCK_H

#include <stdint.h>

// Read the error codes from network
#include "fujinet-network.h"

// IF ADDITIONAL FORMATS ARE ADDED, DO NOT CHANGE THE CURRENT ORDER OF ENUMS
// UNLESS YOU REFACTOR clock_get_time() FUNCTIONS FOR THE PLATFORMS
// AS THEY RELY ON THE ORDER
typedef enum time_format_t {
	// BINARY formats are just the numbers, not ascii characters for the number.
	SIMPLE_BINARY,      // 7 bytes: [Y(century, e.g. 20), Y(hundreds, e.g. 24), M(1-12), D(1-31), H(0-23), M(0-59), S(0-59)] - Uses the current FN Timezone
	PRODOS_BINARY,      // 4 bytes: special PRODOS format, see https://prodos8.com/docs/techref/adding-routines-to-prodos/ - Uses the current FN Timezone
	APETIME_BINARY,     // 6 bytes: [Day, Mon, Yr (YY), Hour, Min, Sec]

	// STRING formats are full null terminated strings
	TZ_ISO_STRING,      // an ISO format: YYYY-MM-DDTHH:MM:SS+HHMM - Uses the current FN Timezone
	UTC_ISO_STRING,     // Current UTC time, still ISO format, but with 0000 offset: YYYY-MM-DDTHH:MM:SS+HHMM

	// APPLE 3 SOS format
	APPLE3_SOS_BINARY   // Apple 3 SOS format, YYYYMMDD0HHMMSS000

} TimeFormat;

/**
 * @brief  Set the FN clock's timezone
 * @param  tz the timezone string to apply
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
uint8_t clock_set_tz(const char *tz);

/**
 * @brief  Get the FN clock's timezone
 * @param  tz pointer to the receiving timezone buffer
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_tz(char *tz);

/**
 * @brief  Get the current time in the format specified using the FN timezone.
 * @param  time_data pointer to buffer for the response. This is uint8_t, but for STRING formats, will be null terminated and can be treated as a string.
 * @param  format a TimeFormat value to specify how the data should be returned.
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);

/**
 * @brief  Get the current time in the format specified for the given timezone without affecting the system timezone
 * @param  time_data pointer to buffer for the response. This is uint8_t, but for STRING formats, will be null terminated and can be treated as a string.
 * @param  tz pointer to the receiving timezone buffer.
 * @param  format a TimeFormat value to specify how the data should be returned.
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_time_tz(uint8_t* time_data, const char* tz, TimeFormat format);

#endif // FUJINET_CLOCK_H
