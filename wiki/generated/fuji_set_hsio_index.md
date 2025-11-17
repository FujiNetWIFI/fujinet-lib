# fuji_set_hsio_index

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Sets HSIO speed index
 * @return success status of request.
 */
bool fuji_set_hsio_index(bool save, uint8_t index);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `save` | `bool` | _TODO: describe parameter_ |
| `index` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Fetch the current HSIO index value.
 * @return success status of request
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_set_hsio_index.c
- 5:bool fuji_set_hsio_index(bool save, uint8_t index)
- 
- atari/src/fn_fuji/fuji_set_hsio_index.s
- 12:; bool fuji_set_hsio_index(bool save, uint8_t index);
- 
- coco/src/fn_fuji/fuji_set_hsio_index.c
- 5:bool fuji_set_hsio_index(bool save, uint8_t index)
- 
- commodore/src/fn_fuji/fuji_set_hsio_index.c
- 6:bool fuji_set_hsio_index(bool save, uint8_t index)
- 
- fujinet-fuji.h
- 505:bool fuji_set_hsio_index(bool save, uint8_t index);
- 
- msdos/src/fn_fuji/fuji_set_hsio_index.c
- 6:bool fuji_set_hsio_index(bool save, uint8_t index)
- 
- pmd85/src/fn_fuji/fuji_set_hsio_index.c
- 3:bool fuji_set_hsio_index(bool save, uint8_t index)


----

[Back to index](../index.md)
