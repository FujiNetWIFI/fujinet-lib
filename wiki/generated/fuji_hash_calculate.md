# fuji_hash_calculate

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief  Calculates the hash of the accumulated data. Can be called multiple times with different hash_type values on the same data if discard_data is false. If different data is required to be hashed, call \ref fuji_hash_clear to start over, then add data with \ref fuji_hash_add again.
 * @param  hash_type The \ref hash_alg_t "type of hash" to use: MD5, SHA1, SHA256, SHA512
 * @param  as_hex True if the returned data should be a hex string, false if it should be binary. Hex has twice the length as binary in the output.
 * @param  discard_data If true the data will be freed from FujiNet memory after the hash is calculated. Use false if you are calculating more than one hash type on the data, and end with discard_data is true, or call \ref fuji_hash_clear to also clean up.
 * @param  output The buffer to write the hash to. The caller is responsible for allocating enough memory for this (based on \ref fuji_hash_length)
 * @return the success status of the operation.
 */
bool fuji_hash_calculate(hash_alg_t hash_type, bool as_hex, bool discard_data, uint8_t *output);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `hash_type` | `hash_alg_t` | _TODO: describe parameter_ |
| `as_hex` | `bool` | _TODO: describe parameter_ |
| `discard_data` | `bool` | _TODO: describe parameter_ |
| `*output` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Adds data that needs to be hashed.
 * @param  data Pointer to the byte data
 * @param  length the length of data to add for hashign
 * @return success status of the operation
 */
```

## Implementations (git grep results)

- common/src/fn_fuji/fuji_hash_calculate.c
- 15:bool fuji_hash_calculate(hash_alg_t hash_type, bool as_hex, bool discard_data, uint8_t *output)
- 
- fujinet-fuji.h
- 661:bool fuji_hash_calculate(hash_alg_t hash_type, bool as_hex, bool discard_data, uint8_t *output);


----

[Back to index](../index.md)
