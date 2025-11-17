# fuji_read_directory_block

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Fill buffer with blocks of directory information.
 * @return success status of request
 * TODO: add full data structure here for people to read.
 */
bool fuji_read_directory_block(uint8_t ram_pages, uint8_t group_size, void *buffer);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `ram_pages` | `uint8_t` | _TODO: describe parameter_ |
| `group_size` | `uint8_t` | _TODO: describe parameter_ |
| `*buffer` | `void` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Fill buffer with directory information.
 * @return success status of request
 */
```

## Implementations (git grep results)

- atari/src/fn_fuji/fuji_read_directory_block.s
- 13:; void *fuji_read_directory_block(uint8_t ram_pages, uint8_t group_size, void *buffer)
- 
- fujinet-fuji.h
- 456:bool fuji_read_directory_block(uint8_t ram_pages, uint8_t group_size, void *buffer);


----

[Back to index](../index.md)
