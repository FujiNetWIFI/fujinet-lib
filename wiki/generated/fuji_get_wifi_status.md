# fuji_get_wifi_status

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief  Sets status to the wifi status value.
 * WL_CONNECTED (3), WL_DISCONNECTED (6) are set if there are no underyling errors in FN.
 * @return Success status, true if all OK.
 */
bool fuji_get_wifi_status(uint8_t *status);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*status` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Checks if WIFI is enabled or not. Any device errors will return false also.
 * @return enabled status 
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_wifi_status.c
- 9:bool fuji_get_wifi_status(uint8_t *status)
- 
- apple2/src/fn_fuji/fuji_get_wifi_status.c
- 5:bool fuji_get_wifi_status(uint8_t *status)
- 
- atari/src/fn_fuji/fuji_get_wifi_status.s
- 11:; bool fuji_get_wifi_status(uint8_t *status)
- 
- coco/src/fn_fuji/fuji_get_wifi_status.c
- 7:bool fuji_get_wifi_status(uint8_t *status)
- 
- commodore/src/fn_fuji/fuji_get_wifi_status.c
- 6:bool fuji_get_wifi_status(uint8_t *status)
- 
- fujinet-fuji.h
- 399:bool fuji_get_wifi_status(uint8_t *status);
- 
- msdos/src/fn_fuji/fuji_get_wifi_status.c
- 6:bool fuji_get_wifi_status(uint8_t *status)
- 
- pmd85/src/fn_fuji/fuji_get_wifi_status.c
- 5:bool fuji_get_wifi_status(uint8_t *status)


----

[Back to index](../index.md)
