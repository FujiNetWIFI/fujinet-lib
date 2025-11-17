# fuji_set_directory_position

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Sets current directory position
 * @return success status of request.
 */
bool fuji_set_directory_position(uint16_t pos);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `pos` | `uint16_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Sends the device/host/mode information for devices to FN
 * @return success status of request.
 */
```

## Implementations (git grep results)

- Changelog.md
- 30:- [coco] CoCo: Fix fuji_set_directory_position (#41) [Rich Stephens]
- 
- adam/src/fn_fuji/fuji_set_directory_position.c
- 9:bool fuji_set_directory_position(uint16_t pos)
- 
- apple2/src/fn_fuji/fuji_set_directory_position.c
- 5:bool fuji_set_directory_position(uint16_t pos)
- 
- atari/src/fn_fuji/fuji_set_directory_position.s
- 11:; bool fuji_set_directory_position(uint16_t pos)
- 
- coco/src/fn_fuji/fuji_set_directory_position.c
- 7:bool fuji_set_directory_position(uint16_t pos)
- 
- commodore/src/fn_fuji/fuji_set_directory_position.c
- 6:bool fuji_set_directory_position(uint16_t pos)
- 
- fujinet-fuji.h
- 492:bool fuji_set_directory_position(uint16_t pos);
- 
- msdos/src/fn_fuji/fuji_set_directory_position.c
- 6:bool fuji_set_directory_position(uint16_t pos)
- 
- pmd85/src/fn_fuji/fuji_set_directory_position.c
- 5:bool fuji_set_directory_position(uint16_t pos)


----

[Back to index](../index.md)
