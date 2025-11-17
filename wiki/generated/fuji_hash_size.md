# fuji_hash_size

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief  Returns the size of the hash that will be produced for the given hash_type and whether hex output is required or not. This should be used to calculate the memory needed for the output of \ref fuji_hash_data
 * @param  hash_type The \ref hash_alg_t "type of hash" to use: MD5, SHA1, SHA256, SHA512
 * @param  as_hex True if the returned data should be a hex string, false if it should be binary. Hex has twice the length as binary in the output.
 * @return the length of the hash that will be computed for this algorithm depending on whether hex is being returned or not.
 */
uint16_t fuji_hash_size(hash_alg_t hash_type, bool as_hex);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `hash_type` | `hash_alg_t` | _TODO: describe parameter_ |
| `as_hex` | `bool` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint16_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * \deprecated Use fuji_hash_size instead, as this function is broken.
 */
```

## Implementations (git grep results)

- common/src/fn_fuji/fuji_hash_calculate.c
- 20:	hash_len = fuji_hash_size(hash_type, as_hex);
- 
- common/src/fn_fuji/fuji_hash_data.c
- 20:	hash_len = fuji_hash_size(hash_type, as_hex);
- 
- common/src/fn_fuji/fuji_hash_size.c
- 11:uint16_t fuji_hash_size(hash_alg_t hash_type, bool is_hex)
- 
- fujinet-fuji.h
- 625:uint16_t fuji_hash_size(hash_alg_t hash_type, bool as_hex);


----

[Back to index](../index.md)
