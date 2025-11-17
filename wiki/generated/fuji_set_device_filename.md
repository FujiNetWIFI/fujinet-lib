# fuji_set_device_filename

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Sends the device/host/mode information for devices to FN
 * @return success status of request.
 */
bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `mode` | `uint8_t` | _TODO: describe parameter_ |
| `hs` | `uint8_t` | _TODO: describe parameter_ |
| `ds` | `uint8_t` | _TODO: describe parameter_ |
| `*buffer` | `char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Sets the booting mode of the FN device (e.g. lobby).
 * @return success status of request.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_set_device_filename.c
- 9:bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
- 
- apple2/src/fn_fuji/fuji_set_device_filename.c
- 6:bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
- 
- atari/src/fn_fuji/fuji_set_device_filename.s
- 13:; bool _fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
- 
- coco/src/fn_fuji/fuji_set_device_filename.c
- 7:bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
- 
- commodore/src/fn_fuji/fuji_set_device_filename.c
- 8:bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
- 
- fujinet-fuji.h
- 486:bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer);
- 
- msdos/src/fn_fuji/fuji_set_device_filename.c
- 6:bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)
- 
- pmd85/src/fn_fuji/fuji_set_device_filename.c
- 6:bool fuji_set_device_filename(uint8_t mode, uint8_t hs, uint8_t ds, char *buffer)


----

[Back to index](../index.md)
