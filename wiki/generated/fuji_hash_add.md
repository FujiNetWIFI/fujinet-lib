# fuji_hash_add

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief  Adds data that needs to be hashed.
 * @param  data Pointer to the byte data
 * @param  length the length of data to add for hashign
 * @return success status of the operation
 */
bool fuji_hash_add(uint8_t *data, uint16_t length);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*data` | `uint8_t` | _TODO: describe parameter_ |
| `length` | `uint16_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Clear any data associated with hashing in the Fujinet. Should be called before calculating new hashes when using \ref fuji_hash_add and \ref fuji_hash_calculate. Can also be called to discard any data previously sent to free memory on the FujiNet used for any previous data sent with \ref fuji_hash_add.
 * @return success status of the operation
 */
```

## Implementations (git grep results)

- common/src/fn_fuji/fuji_hash_add.c
- 15:bool fuji_hash_add(uint8_t *data, uint16_t length)
- 
- fujinet-fuji.h
- 651:bool fuji_hash_add(uint8_t *data, uint16_t length);


----

[Back to index](../index.md)
