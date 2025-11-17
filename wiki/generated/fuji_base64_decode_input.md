# fuji_base64_decode_input

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
bool fuji_base64_decode_input(char *s, uint16_t len);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
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

- adam/src/fn_fuji/fuji_base64_decode_input.c
- 10:bool fuji_base64_decode_input(char *s, uint16_t len)
- 
- apple2/src/fn_fuji/fuji_base64_decode_input.c
- 4:bool fuji_base64_decode_input(char *s, uint16_t len)
- 
- atari/src/fn_fuji/fuji_base64_xxcode_io.s
- 14:; bool fuji_base64_decode_input(char *s, uint16_t len);
- 
- coco/src/fn_fuji/fuji_base64_decode_input.c
- 7:bool fuji_base64_decode_input(char *s, uint16_t len)
- 
- commodore/src/fn_fuji/todo/fuji_base64_decode_input.c
- 6:bool fuji_base64_decode_input(char *s, uint16_t len)
- 
- fujinet-fuji.h
- 580:bool fuji_base64_decode_input(char *s, uint16_t len);
- 
- msdos/src/fn_fuji/fuji_base64_decode_input.c
- 6:bool fuji_base64_decode_input(char *s, uint16_t len)
- 
- pmd85/src/fn_fuji/fuji_base64_decode_input.c
- 5:bool fuji_base64_decode_input(char *s, uint16_t len)


----

[Back to index](../index.md)
