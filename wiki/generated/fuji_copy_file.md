# fuji_copy_file

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Copies a file from given src to dst, with supplied path in copy_spec
 * @return Success status, true if all OK.
 */
bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `src_slot` | `uint8_t` | _TODO: describe parameter_ |
| `dst_slot` | `uint8_t` | _TODO: describe parameter_ |
| `*copy_spec` | `char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Closes the currently open directory
 * @return Success status, true if all OK.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_copy_file.c
- 9:bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
- 
- apple2/src/fn_fuji/fuji_copy_file.c
- 7:bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
- 
- atari/src/fn_fuji/fuji_copy_file.s
- 12:; bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
- 
- coco/src/fn_fuji/fuji_copy_file.c
- 7:bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
- 
- commodore/src/fn_fuji/fuji_copy_file.c
- 8:bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
- 
- fujinet-fuji.h
- 297:bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec);
- 
- msdos/src/fn_fuji/fuji_copy_file.c
- 6:bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)
- 
- pmd85/src/fn_fuji/fuji_copy_file.c
- 5:bool fuji_copy_file(uint8_t src_slot, uint8_t dst_slot, char *copy_spec)


----

[Back to index](../index.md)
