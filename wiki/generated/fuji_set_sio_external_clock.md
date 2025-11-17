# fuji_set_sio_external_clock

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Sets SIO external clock speed
 * @return success status of request.
 */
bool fuji_set_sio_external_clock(uint16_t rate);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `rate` | `uint16_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Sets HSIO speed index
 * @return success status of request.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_set_sio_external_clock.c
- 5:bool fuji_set_sio_external_clock(uint16_t rate)
- 
- atari/src/fn_fuji/fuji_set_sio_external_clock.s
- 11:; bool fuji_set_sio_external_clock(uint16_t rate);
- 
- coco/src/fn_fuji/fuji_set_sio_external_clock.c
- 5:bool fuji_set_sio_external_clock(uint16_t rate)
- 
- fujinet-fuji.h
- 511:bool fuji_set_sio_external_clock(uint16_t rate);
- 
- msdos/src/fn_fuji/fuji_set_sio_external_clock.c
- 6:bool fuji_set_sio_external_clock(uint16_t rate)
- 
- pmd85/src/fn_fuji/fuji_set_sio_external_clock.c
- 3:bool fuji_set_sio_external_clock(uint16_t rate)


----

[Back to index](../index.md)
