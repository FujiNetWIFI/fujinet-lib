# clock_get_time

**Declared in:** `fujinet-clock.h`

## Prototype

```c
/**
 * @brief  Get the current time in the format specified using the FN timezone.
 * @param  time_data pointer to buffer for the response. This is uint8_t, but for STRING formats, will be null terminated and can be treated as a string.
 * @param  format a TimeFormat value to specify how the data should be returned.
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `time_data` | `uint8_t*` | _TODO: describe parameter_ |
| `format` | `TimeFormat` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Get the FN clock's timezone
 * @param  tz pointer to the receiving timezone buffer
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
```

## Implementations (git grep results)

- apple2/apple2-6502/fn_clock/clock_get_time.s
- 52:; uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);
- 
- apple2/apple2gs/fn_clock/clock_get_time.asm
- 1:; uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);
- 
- atari/src/fn_clock/clock_get_time.s
- 50:; uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);
- 
- fujinet-clock.h
- 16:// UNLESS YOU REFACTOR clock_get_time() FUNCTIONS FOR THE PLATFORMS
- 53:uint8_t clock_get_time(uint8_t* time_data, TimeFormat format);


----

[Back to index](../index.md)
