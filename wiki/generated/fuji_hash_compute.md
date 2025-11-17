# fuji_hash_compute

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
////////////////////////////////////////////////////////////////
// These are very low level functions and should only be used internally.
// Please use the new interface functions below this section.
////////////////////////////////////////////////////////////////
// All return success status (true = worked)
bool fuji_hash_compute(uint8_t type);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `type` | `uint8_t` | _TODO: describe parameter_ |

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

- adam/src/fn_fuji/fuji_hash_compute.c
- 8:bool fuji_hash_compute(uint8_t type)
- 
- apple2/src/fn_fuji/fuji_hash_compute.c
- 4:bool fuji_hash_compute(uint8_t type)
- 
- atari/src/fn_fuji/fuji_hash_compute.s
- 12:; bool fuji_hash_compute(uint8_t type);
- 
- coco/src/fn_fuji/fuji_hash_compute.c
- 7:bool fuji_hash_compute(uint8_t type)
- 
- commodore/src/fn_fuji/fuji_hash_compute.c
- 6:bool fuji_hash_compute(uint8_t type)
- 
- common/src/fn_fuji/fuji_hash_calculate.c
- 22:	is_success = discard_data ? fuji_hash_compute(hash_type) : fuji_hash_compute_no_clear(hash_type);
- 
- common/src/fn_fuji/fuji_hash_data.c
- 25:	if (is_success) is_success = fuji_hash_compute(hash_type);
- 
- fujinet-fuji.h
- 594:bool fuji_hash_compute(uint8_t type);
- 
- msdos/src/fn_fuji/fuji_hash_compute.c
- 6:bool fuji_hash_compute(uint8_t type)
- 
- pmd85/src/fn_fuji/fuji_hash_compute.c
- 5:bool fuji_hash_compute(uint8_t type)


----

[Back to index](../index.md)
