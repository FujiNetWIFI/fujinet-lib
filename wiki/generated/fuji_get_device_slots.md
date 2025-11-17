# fuji_get_device_slots

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Sets ALL device slot information into pointer d.
 * `size` is the receiving array size, and the returned data size is checked against this before copying.
 * If it doesn't match, no data is copied, and false is returned.
 * @return Success status, true if all OK.
 */
bool fuji_get_device_slots(DeviceSlot *d, size_t size);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*d` | `DeviceSlot` | _TODO: describe parameter_ |
| `size` | `size_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Sets the buffer to the device's filename in device id `ds`
 * Note: BUFFER MUST BE ABLE TO ACCEPT UP TO 256 BYTE STRING
 * @return Success status, true if all OK.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_device_slots.c
- 9:bool fuji_get_device_slots(DeviceSlot *d, size_t size)
- 
- apple2/src/fn_fuji/fuji_get_device_slots.c
- 9:bool fuji_get_device_slots(DeviceSlot *d, size_t size)
- 
- atari/src/fn_fuji/fuji_get_device_slots.s
- 14:; bool fuji_get_device_slots(DeviceSlot *device_slots, size_t size)
- 
- coco/src/fn_fuji/fuji_get_device_slots.c
- 7:bool fuji_get_device_slots(DeviceSlot *d, size_t size)
- 
- commodore/src/fn_fuji/fuji_get_device_slots.c
- 8:bool fuji_get_device_slots(DeviceSlot *d, size_t size)
- 
- fujinet-fuji.h
- 352:bool fuji_get_device_slots(DeviceSlot *d, size_t size);
- 
- msdos/src/fn_fuji/fuji_get_device_slots.c
- 6:bool fuji_get_device_slots(DeviceSlot *d, size_t size)
- 
- pmd85/src/fn_fuji/fuji_get_device_slots.c
- 5:bool fuji_get_device_slots(DeviceSlot *d, size_t size)


----

[Back to index](../index.md)
