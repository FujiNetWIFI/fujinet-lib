# clock_get_time_tz

**Declared in:** `fujinet-clock.h`

## Prototype

```c
/**
 * @brief  Get the current time in the format specified for the given timezone without affecting the system timezone
 * @param  time_data pointer to buffer for the response. This is uint8_t, but for STRING formats, will be null terminated and can be treated as a string.
 * @param  tz pointer to the receiving timezone buffer.
 * @param  format a TimeFormat value to specify how the data should be returned.
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_time_tz(uint8_t* time_data, const char* tz, TimeFormat format);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `time_data` | `uint8_t*` | _TODO: describe parameter_ |
| `tz` | `const char*` | _TODO: describe parameter_ |
| `format` | `TimeFormat` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Get the current time in the format specified using the FN timezone.
 * @param  time_data pointer to buffer for the response. This is uint8_t, but for STRING formats, will be null terminated and can be treated as a string.
 * @param  format a TimeFormat value to specify how the data should be returned.
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
```

## Implementations (git grep results)

- apple2/apple2-6502/fn_clock/clock_get_time.s
- 26:; uint8_t clock_get_time_tz(uint8_t* time_data, const char* tz, TimeFormat format);
- 
- atari/src/fn_clock/clock_get_time.s
- 21:; uint8_t clock_get_time_tz(uint8_t* time_data, const char* tz, TimeFormat format);
- 
- fujinet-clock.h
- 62:uint8_t clock_get_time_tz(uint8_t* time_data, const char* tz, TimeFormat format);


----

[Back to index](../index.md)
