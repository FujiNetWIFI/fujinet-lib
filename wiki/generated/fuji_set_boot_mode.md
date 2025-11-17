# fuji_set_boot_mode

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Sets the booting mode of the FN device (e.g. lobby).
 * @return success status of request.
 */
bool fuji_set_boot_mode(uint8_t mode);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `mode` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Scans network for SSIDs and sets count accordingly.
 * @return success status of request.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_set_boot_mode.c
- 8:bool fuji_set_boot_mode(uint8_t mode)
- 
- apple2/src/fn_fuji/fuji_set_boot_mode.c
- 5:bool fuji_set_boot_mode(uint8_t mode)
- 
- atari/src/fn_fuji/fuji_set_boot_mode.s
- 11:; void _fuji_set_boot_mode(uint8_t mode)
- 
- coco/src/fn_fuji/fuji_set_boot_mode.c
- 7:bool fuji_set_boot_mode(uint8_t mode)
- 
- commodore/src/fn_fuji/fuji_set_boot_mode.c
- 6:bool fuji_set_boot_mode(uint8_t mode)
- 
- fujinet-fuji.h
- 480:bool fuji_set_boot_mode(uint8_t mode);
- 
- msdos/src/fn_fuji/fuji_set_boot_mode.c
- 6:bool fuji_set_boot_mode(uint8_t mode)
- 
- pmd85/src/fn_fuji/fuji_set_boot_mode.c
- 5:bool fuji_set_boot_mode(uint8_t mode)


----

[Back to index](../index.md)
