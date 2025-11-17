# fuji_base64_decode_length

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
bool fuji_base64_decode_length(unsigned long *len);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*len` | `unsigned long` | _TODO: describe parameter_ |

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

- adam/src/fn_fuji/fuji_base64_decode_length.c
- 9:bool fuji_base64_decode_length(unsigned long *len)
- 
- apple2/src/fn_fuji/fuji_base64_decode_length.c
- 4:bool fuji_base64_decode_length(unsigned long *len)
- 
- atari/src/fn_fuji/fuji_base64_xxcode_length.s
- 13:; bool fuji_base64_decode_length(unsigned long *len);
- 
- coco/src/fn_fuji/fuji_base64_decode_length.c
- 7:bool fuji_base64_decode_length(unsigned long *len)
- 
- commodore/src/fn_fuji/todo/fuji_base64_decode_length.c
- 6:bool fuji_base64_decode_length(unsigned long *len)
- 
- fujinet-fuji.h
- 581:bool fuji_base64_decode_length(unsigned long *len);
- 
- msdos/src/fn_fuji/fuji_base64_decode_length.c
- 6:bool fuji_base64_decode_length(unsigned long *len)
- 
- pmd85/src/fn_fuji/fuji_base64_decode_length.c
- 5:bool fuji_base64_decode_length(unsigned long *len)


----

[Back to index](../index.md)
