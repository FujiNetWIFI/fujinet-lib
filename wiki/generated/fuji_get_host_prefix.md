# fuji_get_host_prefix

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Fetch the host prefix for given host slot id.
 * @return success status of request
 */
bool fuji_get_host_prefix(uint8_t hs, char *prefix);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `hs` | `uint8_t` | _TODO: describe parameter_ |
| `*prefix` | `char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Fetch the current directory position for paging through directories into pos.
 * @return success status of request
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_host_prefix.c
- 8:bool fuji_get_host_prefix(uint8_t hs, char *prefix)
- 
- apple2/src/fn_fuji/fuji_get_host_prefix.c
- 4:bool fuji_get_host_prefix(uint8_t hs, char *prefix)
- 
- atari/src/fn_fuji/fuji_host_prefix.s
- 14:; bool fuji_get_host_prefix(uint8_t hs, char *prefix);
- 
- coco/src/fn_fuji/fuji_get_host_prefix.c
- 5:bool fuji_get_host_prefix(uint8_t hs, char *prefix)
- 
- commodore/src/fn_fuji/fuji_get_host_prefix.c
- 6:bool fuji_get_host_prefix(uint8_t hs, char *prefix)
- 
- fujinet-fuji.h
- 364:bool fuji_get_host_prefix(uint8_t hs, char *prefix);
- 
- msdos/src/fn_fuji/fuji_get_host_prefix.c
- 6:bool fuji_get_host_prefix(uint8_t hs, char *prefix)
- 
- pmd85/src/fn_fuji/fuji_get_host_prefix.c
- 3:bool fuji_get_host_prefix(uint8_t hs, char *prefix)


----

[Back to index](../index.md)
