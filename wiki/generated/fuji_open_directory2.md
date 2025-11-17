# fuji_open_directory2

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Open the given directory on the given host slot.
 * path and filter are separate strings. If filter is not set, it is NULL
 * @return true if successful, false otherwise
 */
bool fuji_open_directory2(uint8_t hs, char *path, char *filter);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `hs` | `uint8_t` | _TODO: describe parameter_ |
| `*path` | `char` | _TODO: describe parameter_ |
| `*filter` | `char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Open the given directory on the given host slot.
 * The path_filter is a buffer (not a string) of 256 bytes, with a separator of the \0 char between the path and filter parts.
 * @return true if successful, false otherwise
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_open_directory2.c
- 5:bool fuji_open_directory2(uint8_t hs, char *path, char *filter)
- 
- coco/src/fn_fuji/fuji_open_directory2.c
- 7:bool fuji_open_directory2(uint8_t hs, char *path, char *filter)
- 
- commodore/src/fn_fuji/fuji_open_directory2.c
- 8:bool fuji_open_directory2(uint8_t hs, char *path, char *filter)
- 
- fujinet-fuji.h
- 431:bool fuji_open_directory2(uint8_t hs, char *path, char *filter);
- 
- msdos/src/fn_fuji/fuji_open_directory2.c
- 6:bool fuji_open_directory2(uint8_t hs, char *path, char *filter)
- 
- pmd85/src/fn_fuji/fuji_open_directory2.c
- 6:bool fuji_open_directory2(uint8_t hs, char *path, char *filter)


----

[Back to index](../index.md)
