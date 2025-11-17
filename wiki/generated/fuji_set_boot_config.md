# fuji_set_boot_config

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Scans network for SSIDs and sets count accordingly.
 * @return success status of request.
 */
bool fuji_set_boot_config(uint8_t toggle);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `toggle` | `uint8_t` | _TODO: describe parameter_ |

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

- adam/src/fn_fuji/fuji_set_boot_config.c
- 8:bool fuji_set_boot_config(uint8_t toggle)
- 
- apple2/src/fn_fuji/fuji_set_boot_config.c
- 5:bool fuji_set_boot_config(uint8_t toggle)
- 
- atari/src/fn_fuji/fuji_set_boot_config.s
- 11:; bool fuji_set_boot_config(uint8_t toggle)
- 
- coco/src/fn_fuji/fuji_set_boot_config.c
- 7:bool fuji_set_boot_config(uint8_t toggle)
- 
- commodore/src/fn_fuji/fuji_set_boot_config.c
- 6:bool fuji_set_boot_config(uint8_t toggle)
- 
- fujinet-fuji.h
- 474:bool fuji_set_boot_config(uint8_t toggle);
- 
- msdos/src/fn_fuji/fuji_set_boot_config.c
- 6:bool fuji_set_boot_config(uint8_t toggle)
- 
- pmd85/src/fn_fuji/fuji_set_boot_config.c
- 5:bool fuji_set_boot_config(uint8_t toggle)


----

[Back to index](../index.md)
