# fuji_unmount_host_slot

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Unmounts the host in slot hs
 * @return success status of request
 */
bool fuji_unmount_host_slot(uint8_t hs);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `hs` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Unmounts the device in slot ds
 * @return success status of request
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_unmount_host_slot.c
- 8:bool fuji_unmount_host_slot(uint8_t hs)
- 
- apple2/src/fn_fuji/fuji_unmount_host_slot.c
- 5:bool fuji_unmount_host_slot(uint8_t hs)
- 
- atari/src/fn_fuji/fuji_unmount_host_slot.s
- 11:; bool _fuji_unmount_host_slot(uint8_t hs)
- 
- coco/src/fn_fuji/fuji_unmount_host_slot.c
- 7:bool fuji_unmount_host_slot(uint8_t hs)
- 
- commodore/src/fn_fuji/fuji_unmount_host_slot.c
- 6:bool fuji_unmount_host_slot(uint8_t hs)
- 
- fujinet-fuji.h
- 549:bool fuji_unmount_host_slot(uint8_t hs);
- 
- msdos/src/fn_fuji/fuji_unmount_host_slot.c
- 6:bool fuji_unmount_host_slot(uint8_t hs)
- 
- pmd85/src/fn_fuji/fuji_unmount_host_slot.c
- 5:bool fuji_unmount_host_slot(uint8_t hs)


----

[Back to index](../index.md)
