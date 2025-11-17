# fuji_hash_output

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
// this requires compute to have been called to set the hashing algorithm - don't use!
// ALSO, there is no way to get the return value with this signature

// output_type is 1 for hex, 0 of binary. the len is the length of data, which currently is only up to 128 if hex, but future proofing with word.
bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `output_type` | `uint8_t` | _TODO: describe parameter_ |
| `*s` | `char` | _TODO: describe parameter_ |
| `len` | `uint16_t` | _TODO: describe parameter_ |

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

- adam/src/fn_fuji/fuji_hash_output.c
- 10:bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len)
- 
- apple2/src/fn_fuji/fuji_hash_output.c
- 4:bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len)
- 
- atari/src/fn_fuji/fuji_hash_output.s
- 12:; bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len);
- 
- coco/src/fn_fuji/fuji_hash_output.c
- 7:bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len)
- 
- commodore/src/fn_fuji/fuji_hash_output.c
- 6:bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len)
- 
- common/src/fn_fuji/fuji_hash_calculate.c
- 26:	if (is_success) is_success = fuji_hash_output((uint8_t) as_hex, (char *) output, hash_len);
- 
- common/src/fn_fuji/fuji_hash_data.c
- 28:	if (is_success) is_success = fuji_hash_output((uint8_t) as_hex, (char *) output, hash_len);
- 
- fujinet-fuji.h
- 601:bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len);
- 
- msdos/src/fn_fuji/fuji_hash_output.c
- 6:bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len)
- 
- pmd85/src/fn_fuji/fuji_hash_output.c
- 5:bool fuji_hash_output(uint8_t output_type, char *s, uint16_t len)


----

[Back to index](../index.md)
