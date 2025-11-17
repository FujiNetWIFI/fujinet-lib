# fuji_get_directory_position

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Fetch the current directory position for paging through directories into pos.
 * @return success status of request
 */
bool fuji_get_directory_position(uint16_t *pos);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*pos` | `uint16_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Sets ALL device slot information into pointer d.
 * `size` is the receiving array size, and the returned data size is checked against this before copying.
 * If it doesn't match, no data is copied, and false is returned.
 * @return Success status, true if all OK.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_directory_position.c
- 9:bool fuji_get_directory_position(uint16_t *pos)
- 
- apple2/src/fn_fuji/fuji_get_directory_position.c
- 5:bool fuji_get_directory_position(uint16_t *pos)
- 
- atari/src/fn_fuji/fuji_get_directory_position.s
- 13:; bool fuji_get_directory_position(uint16_t *pos);
- 
- coco/src/fn_fuji/fuji_get_directory_position.c
- 7:bool fuji_get_directory_position(uint16_t *pos)
- 
- commodore/src/fn_fuji/fuji_get_directory_position.c
- 6:bool fuji_get_directory_position(uint16_t *pos)
- 
- fujinet-fuji.h
- 358:bool fuji_get_directory_position(uint16_t *pos);
- 
- msdos/src/fn_fuji/fuji_get_directory_position.c
- 6:bool fuji_get_directory_position(uint16_t *pos)
- 
- pmd85/src/fn_fuji/fuji_get_directory_position.c
- 5:bool fuji_get_directory_position(uint16_t *pos)


----

[Back to index](../index.md)
