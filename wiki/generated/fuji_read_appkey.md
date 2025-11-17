# fuji_read_appkey

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief  Opens and reads from appkey using the provided details. Writes to the data buffer, and sets count to the amount of data read if it is less than the full keysize.
 * @param  key_id the specific key id of this application.
 * @param  count a pointer to an int for the number of bytes that were read.
 * @param  data a pointer to the memory to write the data back to. WARNING: The data buffer needs to be at least 2 more bytes larger than the keysize.
 * @return success status of the call. If either the initial OPEN or subsequent READ fail, will return false.
 */
bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `key_id` | `uint8_t` | _TODO: describe parameter_ |
| `*count` | `uint16_t` | _TODO: describe parameter_ |
| `*data` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Unmounts the host in slot hs
 * @return success status of request
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_read_appkey.c
- 12:bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
- 
- apple2/src/fn_fuji/fuji_appkey_open_read.c
- 11:bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
- 
- atari/src/fn_fuji/fuji_appkey_open_read.c
- 16:bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
- 
- coco/src/fn_fuji/fuji_read_appkey.c
- 12:bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
- 
- commodore/src/fn_fuji/fuji_appkey_open_read.c
- 10:bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
- 
- fujinet-fuji.h
- 558:bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data);
- 
- msdos/src/fn_fuji/fuji_read_appkey.c
- 6:bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)
- 
- pmd85/src/fn_fuji/fuji_read_appkey.c
- 11:bool fuji_read_appkey(uint8_t key_id, uint16_t *count, uint8_t *data)


----

[Back to index](../index.md)
