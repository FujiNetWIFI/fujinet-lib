# fuji_hash_data

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief  Computes the hash of the given input data in a single operation. This is a simpler interface than using \ref fuji_hash_clear, \ref fuji_hash_add, \ref fuji_hash_calculate, and can be used instead of those 3 operations if there is only 1 piece of data to hash.
 *         This will also clear any data previously add using fuji_hash_add, and also clears any memory associated with hashing in the FujiNet at the end of the operation. It is the one stop shop, if you use it with the other 3 functions, you must sequence them correctly so this function doesn't clear the existing sent data.
 * @param  hash_type The \ref hash_alg_t "type of hash" to use: MD5, SHA1, SHA256, SHA512
 * @param  input The binary data that requires hash computed on
 * @param  length The length of binary data in "input" to compute a hash on
 * @param  as_hex True if the returned data should be a hex string, false if it should be binary. Hex has twice the length as binary in the output.
 * @param  output The buffer to write the hash value to. This must be allocated by the application itself, it is not done in the library. \ref fuji_hash_value "fuji_hash_value()".
 * @returns sucess status of the operation
 */
bool fuji_hash_data(hash_alg_t hash_type, uint8_t *input, uint16_t length, bool as_hex, uint8_t *output);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `hash_type` | `hash_alg_t` | _TODO: describe parameter_ |
| `*input` | `uint8_t` | _TODO: describe parameter_ |
| `length` | `uint16_t` | _TODO: describe parameter_ |
| `as_hex` | `bool` | _TODO: describe parameter_ |
| `*output` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Returns the size of the hash that will be produced for the given hash_type and whether hex output is required or not. This should be used to calculate the memory needed for the output of \ref fuji_hash_data
 * @param  hash_type The \ref hash_alg_t "type of hash" to use: MD5, SHA1, SHA256, SHA512
 * @param  as_hex True if the returned data should be a hex string, false if it should be binary. Hex has twice the length as binary in the output.
 * @return the length of the hash that will be computed for this algorithm depending on whether hex is being returned or not.
 */
```

## Implementations (git grep results)

- common/src/fn_fuji/fuji_hash_data.c
- 15:bool fuji_hash_data(hash_alg_t hash_type, uint8_t *input, uint16_t length, bool as_hex, uint8_t *output)
- 
- fujinet-fuji.h
- 637:bool fuji_hash_data(hash_alg_t hash_type, uint8_t *input, uint16_t length, bool as_hex, uint8_t *output);


----

[Back to index](../index.md)
