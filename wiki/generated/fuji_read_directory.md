# fuji_read_directory

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Fill buffer with directory information.
 * @return success status of request
 */
bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `maxlen` | `uint8_t` | _TODO: describe parameter_ |
| `aux2` | `uint8_t` | _TODO: describe parameter_ |
| `*buffer` | `char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Save `size` hosts slots to FN
 * @return true if successful, false if there was an error from FN
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_read_directory.c
- 10:bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
- 
- apple2/src/fn_fuji/fuji_read_directory.c
- 6:bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
- 
- atari/src/fn_fuji/fuji_read_directory.s
- 12:; bool fuji_read_directory(unsigned char maxlen, unsigned char aux2, char *buffer)
- 
- coco/src/fn_fuji/fuji_read_directory.c
- 7:bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
- 
- commodore/src/fn_fuji/fuji_read_directory.c
- 6:bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
- 
- fujinet-fuji.h
- 449:bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer);
- 
- msdos/src/fn_fuji/fuji_read_directory.c
- 6:bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)
- 
- pmd85/src/fn_fuji/fuji_read_directory.c
- 5:bool fuji_read_directory(uint8_t maxlen, uint8_t aux2, char *buffer)


----

[Back to index](../index.md)
