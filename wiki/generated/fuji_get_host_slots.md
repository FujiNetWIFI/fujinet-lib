# fuji_get_host_slots

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Sets ALL host slot information into pointer h.
 * `size` is the number of host slots, and the returned data size is checked against this multiple of HostSlot structs before copying.
 * If it doesn't match, no data is copied, and false is returned.
 * @return Success status, true if all OK.
 */
bool fuji_get_host_slots(HostSlot *h, size_t size);
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
 * @brief Fetch the host prefix for given host slot id.
 * @return success status of request
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_host_slots.c
- 9:bool fuji_get_host_slots(HostSlot *h, size_t size)
- 
- apple2/src/fn_fuji/fuji_get_host_slots.c
- 8:bool fuji_get_host_slots(HostSlot *h, size_t size)
- 
- atari/src/fn_fuji/fuji_get_host_slots.s
- 13:; bool fuji_get_host_slots(struct HostSlot *host_slots, size_t size)
- 
- coco/src/fn_fuji/fuji_get_host_slots.c
- 7:bool fuji_get_host_slots(HostSlot *h, size_t size)
- 
- commodore/src/fn_fuji/fuji_get_device_slots.c
- 15:	// see comments in fuji_get_host_slots().
- 
- commodore/src/fn_fuji/fuji_get_host_slots.c
- 8:bool fuji_get_host_slots(HostSlot *h, size_t size)
- 
- fujinet-fuji.h
- 372:bool fuji_get_host_slots(HostSlot *h, size_t size);
- 
- msdos/src/fn_fuji/fuji_get_host_slots.c
- 6:bool fuji_get_host_slots(HostSlot *h, size_t size)
- 
- pmd85/src/fn_fuji/fuji_get_host_slots.c
- 5:bool fuji_get_host_slots(HostSlot *h, size_t size)


----

[Back to index](../index.md)
