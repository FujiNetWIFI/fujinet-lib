# fuji_open_directory

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Open the given directory on the given host slot.
 * The path_filter is a buffer (not a string) of 256 bytes, with a separator of the \0 char between the path and filter parts.
 * @return true if successful, false otherwise
 */
bool fuji_open_directory(uint8_t hs, char *path_filter);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `hs` | `uint8_t` | _TODO: describe parameter_ |
| `*path_filter` | `char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Mount specific host slot
 * @return true if successful, false otherwise
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_open_directory.c
- 9:bool fuji_open_directory(uint8_t hs, char *path_filter)
- 
- apple2/src/fn_fuji/fuji_open_directory.c
- 6:bool fuji_open_directory(uint8_t hs, char *path_filter)
- 
- atari/src/fn_fuji/fuji_open_directory.s
- 12:; bool fuji_open_directory(uint8_t host_slot, char *path_filter)
- 
- coco/src/fn_fuji/fuji_open_directory.c
- 7:bool fuji_open_directory(uint8_t hs, char *path_filter)
- 
- commodore/src/fn_fuji/fuji_open_directory.c
- 5:bool fuji_open_directory(uint8_t hs, char *path_filter)
- 
- fujinet-fuji.h
- 424:bool fuji_open_directory(uint8_t hs, char *path_filter);
- 
- msdos/src/fn_fuji/fuji_open_directory.c
- 6:bool fuji_open_directory(uint8_t hs, char *path_filter)
- 
- pmd85/src/fn_fuji/fuji_open_directory.c
- 6:bool fuji_open_directory(uint8_t hs, char *path_filter)


----

[Back to index](../index.md)
