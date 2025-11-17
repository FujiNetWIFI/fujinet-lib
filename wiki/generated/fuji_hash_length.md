# fuji_hash_length

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * \deprecated Use fuji_hash_size instead, as this function is broken.
 */
bool fuji_hash_length(uint8_t mode);
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
 * @brief  Sets the base details for appkeys. This must be called before using read or write operations on appkeys.
 * @param  creator_id the id of the creator of the appkey
 * @param  app_id the id of the application from the creator
 * @param  keysize type: AppKeySize, set to DEFAULT for 64 byte appkeys, or SIZE_256 for 256 byte keys
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_hash_length.c
- 10:bool fuji_hash_length(uint8_t mode)
- 
- apple2/src/fn_fuji/fuji_hash_length.c
- 4:bool fuji_hash_length(uint8_t mode)
- 
- atari/src/fn_fuji/fuji_hash_length.s
- 14:; bool fuji_hash_length(uint8_t type);
- 
- coco/src/fn_fuji/fuji_hash_length.c
- 8:bool fuji_hash_length(uint8_t mode)
- 
- commodore/src/fn_fuji/fuji_hash_length.c
- 9:bool fuji_hash_length(uint8_t mode)
- 
- fujinet-fuji.h
- 606:bool fuji_hash_length(uint8_t mode);
- 
- msdos/src/fn_fuji/fuji_hash_length.c
- 6:bool fuji_hash_length(uint8_t mode)
- 
- pmd85/src/fn_fuji/fuji_hash_length.c
- 6:bool fuji_hash_length(uint8_t mode)


----

[Back to index](../index.md)
