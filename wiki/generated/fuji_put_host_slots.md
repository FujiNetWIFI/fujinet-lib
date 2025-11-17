# fuji_put_host_slots

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Save `size` hosts slots to FN
 * @return true if successful, false if there was an error from FN
 */
bool fuji_put_host_slots(HostSlot *h, size_t size);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*h` | `HostSlot` | _TODO: describe parameter_ |
| `size` | `size_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Save `size` device slots to FN
 * @return true if successful, false if there was an error from FN
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_put_host_slots.c
- 9:bool fuji_put_host_slots(HostSlot *h, size_t size)
- 
- apple2/src/fn_fuji/fuji_put_host_slots.c
- 6:bool fuji_put_host_slots(HostSlot *h, size_t size)
- 
- atari/src/fn_fuji/fuji_put_host_slots.s
- 13:; bool _fuji_put_host_slots(HostSlot *h, size_t size)
- 
- coco/src/fn_fuji/fuji_put_host_slots.c
- 7:bool fuji_put_host_slots(HostSlot *h, size_t size)
- 
- commodore/src/fn_fuji/fuji_put_host_slots.c
- 8:bool fuji_put_host_slots(HostSlot *h, size_t size)
- 
- fujinet-fuji.h
- 443:bool fuji_put_host_slots(HostSlot *h, size_t size);
- 
- msdos/src/fn_fuji/fuji_put_host_slots.c
- 6:bool fuji_put_host_slots(HostSlot *h, size_t size)
- 
- pmd85/src/fn_fuji/fuji_put_host_slots.c
- 5:bool fuji_put_host_slots(HostSlot *h, size_t size)


----

[Back to index](../index.md)
