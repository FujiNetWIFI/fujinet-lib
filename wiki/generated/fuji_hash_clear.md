# fuji_hash_clear

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief  Clear any data associated with hashing in the Fujinet. Should be called before calculating new hashes when using \ref fuji_hash_add and \ref fuji_hash_calculate. Can also be called to discard any data previously sent to free memory on the FujiNet used for any previous data sent with \ref fuji_hash_add.
 * @return success status of the operation
 */
bool fuji_hash_clear(void);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

_This function takes no parameters._

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
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
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_hash_clear.c
- 8:bool fuji_hash_clear()
- 
- atari/src/fn_fuji/fuji_hash_clear.s
- 10:; bool fuji_hash_clear();
- 
- coco/src/fn_fuji/fuji_hash_clear.c
- 7:bool fuji_hash_clear()
- 
- commodore/src/fn_fuji/fuji_hash_clear.c
- 6:bool fuji_hash_clear()
- 
- common/src/fn_fuji/fuji_hash_calculate.c
- 28:	if (discard_data) fuji_hash_clear();
- 
- common/src/fn_fuji/fuji_hash_data.c
- 21:	is_success = fuji_hash_clear();
- 
- fujinet-fuji.h
- 643:bool fuji_hash_clear(void);
- 
- msdos/src/fn_fuji/fuji_hash_clear.c
- 6:bool fuji_hash_clear(void)
- 
- pmd85/src/fn_fuji/fuji_hash_clear.c
- 5:bool fuji_hash_clear()


----

[Back to index](../index.md)
