# fuji_mount_disk_image

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Mount specific device slot
 * @return true if successful, false otherwise
 */
bool fuji_mount_disk_image(uint8_t ds, uint8_t mode);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `ds` | `uint8_t` | _TODO: describe parameter_ |
| `mode` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Mount all devices
 * @return true if successful, false otherwise
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_mount_disk_image.c
- 8:bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
- 
- apple2/src/fn_fuji/fuji_mount_disk_image.c
- 5:bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
- 
- atari/src/fn_fuji/fuji_mount_disk_image.s
- 12:; bool fuji_mount_disk_image(uint8_t device_slot, uint8_t mode)
- 
- coco/src/fn_fuji/fuji_mount_disk_image.c
- 7:bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
- 
- commodore/src/fn_fuji/fuji_mount_disk_image.c
- 6:bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
- 
- fujinet-fuji.h
- 411:bool fuji_mount_disk_image(uint8_t ds, uint8_t mode);
- 
- msdos/src/fn_fuji/fuji_mount_disk_image.c
- 6:bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)
- 
- pmd85/src/fn_fuji/fuji_mount_disk_image.c
- 5:bool fuji_mount_disk_image(uint8_t ds, uint8_t mode)


----

[Back to index](../index.md)
