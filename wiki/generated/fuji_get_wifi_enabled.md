# fuji_get_wifi_enabled

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Checks if WIFI is enabled or not. Any device errors will return false also.
 * @return enabled status 
 */
bool fuji_get_wifi_enabled(void);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

_This function takes no parameters._

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Fills net_config with current SSID/password values.
 * No data copied if there is an error.
 * @return Success status, true if all OK.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_wifi_enabled.c
- 9:bool fuji_get_wifi_enabled()
- 
- apple2/src/fn_fuji/fuji_get_wifi_enabled.c
- 5:bool fuji_get_wifi_enabled(void)
- 
- atari/src/fn_fuji/fuji_get_wifi_enabled.s
- 13:; int fuji_get_wifi_enabled()
- 
- coco/src/fn_fuji/fuji_get_wifi_enabled.c
- 7:bool fuji_get_wifi_enabled()
- 
- commodore/src/fn_fuji/fuji_get_wifi_enabled.c
- 6:bool fuji_get_wifi_enabled()
- 
- fujinet-fuji.h
- 392:bool fuji_get_wifi_enabled(void);
- 
- msdos/src/fn_fuji/fuji_get_wifi_enabled.c
- 6:bool fuji_get_wifi_enabled(void)
- 
- pmd85/src/fn_fuji/fuji_get_wifi_enabled.c
- 5:bool fuji_get_wifi_enabled()


----

[Back to index](../index.md)
