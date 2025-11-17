# clock_set_tz

**Declared in:** `fujinet-clock.h`

## Prototype

```c
/**
 * @brief  Set the FN clock's timezone
 * @param  tz the timezone string to apply
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
uint8_t clock_set_tz(const char *tz);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*tz` | `const char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief FujiNet Clock Device Library
 * @license gpl v. 3, see LICENSE for details.
 */
```

## Implementations (git grep results)

- apple2/apple2-6502/fn_clock/clock_set_tz.s
- 27:; uint8_t clock_set_tz(char *tz);
- 
- apple2/apple2gs/fn_clock/clock_set_tz.asm
- 1:; uint8_t clock_set_tz(char *tz);
- 
- atari/src/fn_clock/clock_set_tz.s
- 23:; uint8_t clock_set_tz(const char *tz);
- 
- fujinet-clock.h
- 38:uint8_t clock_set_tz(const char *tz);


----

[Back to index](../index.md)
