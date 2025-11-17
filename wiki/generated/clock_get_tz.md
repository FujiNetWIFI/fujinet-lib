# clock_get_tz

**Declared in:** `fujinet-clock.h`

## Prototype

```c
/**
 * @brief  Get the FN clock's timezone
 * @param  tz pointer to the receiving timezone buffer
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
uint8_t clock_get_tz(char *tz);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*tz` | `char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Set the FN clock's timezone
 * @param  tz the timezone string to apply
 * @return fujinet-clock status/error code (See FN_ERR_* values)
 */
```

## Implementations (git grep results)

- apple2/apple2-6502/fn_clock/clock_get_tz.s
- 15:; uint8_t clock_get_tz(uint8_t* tz);
- 
- apple2/apple2gs/fn_clock/clock_get_tz.asm
- 1:; uint8_t clock_get_tz(uint8_t* tz);
- 
- atari/src/fn_clock/clock_get_tz.s
- 14:; uint8_t clock_get_tz(uint8_t* tz);
- 
- fujinet-clock.h
- 45:uint8_t clock_get_tz(char *tz);


----

[Back to index](../index.md)
