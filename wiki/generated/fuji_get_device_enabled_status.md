# fuji_get_device_enabled_status

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * THIS IS BOGUS. Apple and Atari both just return "true" for any device.
 * TODO: remove this if it isn't going to be properly implemented on FN.
 *       which I think it could be by looking on FN side by looking at Config
 * @return the enabled status of the given device: NOTE: ALWAYS RETURNS TRUE AT MOMENT
 */
bool fuji_get_device_enabled_status(uint8_t d);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `d` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Gets extended adapter config information from FN, e.g. IP, MAC, BSSID etc.
 * Extended version that returns strings in addition to raw for all IP etc related values.
 * @return Success status, true if all OK.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_device_enabled_status.c
- 9:bool fuji_get_device_enabled_status(uint8_t d)
- 
- apple2/src/fn_fuji/fuji_get_device_enabled_status.c
- 7:bool fuji_get_device_enabled_status(uint8_t d)
- 
- coco/src/fn_fuji/fuji_get_device_enabled_status.c
- 5:bool fuji_get_device_enabled_status(uint8_t d)
- 
- commodore/src/fn_fuji/fuji_get_device_enabled_status.c
- 7:bool fuji_get_device_enabled_status(uint8_t d)
- 
- fujinet-fuji.h
- 337:bool fuji_get_device_enabled_status(uint8_t d);
- 
- msdos/src/fn_fuji/fuji_get_device_enabled_status.c
- 6:bool fuji_get_device_enabled_status(uint8_t d)
- 
- pmd85/src/fn_fuji/fuji_get_device_enabled_status.c
- 3:bool fuji_get_device_enabled_status(uint8_t d)


----

[Back to index](../index.md)
