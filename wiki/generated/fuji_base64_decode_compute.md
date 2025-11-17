# fuji_base64_decode_compute

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
// Base64
// ALL RETURN VALUES ARE SUCCESS STATUS VALUE, i.e. true == success
bool fuji_base64_decode_compute(void);
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
 * @brief  Sets the base details for appkeys. This must be called before using read or write operations on appkeys.
 * @param  creator_id the id of the creator of the appkey
 * @param  app_id the id of the application from the creator
 * @param  keysize type: AppKeySize, set to DEFAULT for 64 byte appkeys, or SIZE_256 for 256 byte keys
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_base64_decode_compute.c
- 8:bool fuji_base64_decode_compute()
- 
- apple2/src/fn_fuji/fuji_base64_decode_compute.c
- 4:bool fuji_base64_decode_compute(void)
- 
- atari/src/fn_fuji/fuji_base64_xxcode_compute.s
- 13:; bool fuji_base64_decode_compute();
- 
- coco/src/fn_fuji/fuji_base64_decode_compute.c
- 7:bool fuji_base64_decode_compute()
- 
- commodore/src/fn_fuji/todo/fuji_base64_decode_compute.c
- 6:bool fuji_base64_decode_compute()
- 
- fujinet-fuji.h
- 579:bool fuji_base64_decode_compute(void);
- 
- msdos/src/fn_fuji/fuji_base64_decode_compute.c
- 6:bool fuji_base64_decode_compute(void)
- 
- pmd85/src/fn_fuji/fuji_base64_decode_compute.c
- 5:bool fuji_base64_decode_compute()


----

[Back to index](../index.md)
