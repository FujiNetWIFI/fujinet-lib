# fuji_get_device_filename

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Sets the buffer to the device's filename in device id `ds`
 * Note: BUFFER MUST BE ABLE TO ACCEPT UP TO 256 BYTE STRING
 * @return Success status, true if all OK.
 */
bool fuji_get_device_filename(uint8_t ds, char *buffer);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `ds` | `uint8_t` | _TODO: describe parameter_ |
| `*buffer` | `char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * THIS IS BOGUS. Apple and Atari both just return "true" for any device.
 * TODO: remove this if it isn't going to be properly implemented on FN.
 *       which I think it could be by looking on FN side by looking at Config
 * @return the enabled status of the given device: NOTE: ALWAYS RETURNS TRUE AT MOMENT
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_device_filename.c
- 10:bool fuji_get_device_filename(uint8_t ds, char *buffer)
- 
- apple2/src/fn_fuji/fuji_get_device_filename.c
- 8:bool fuji_get_device_filename(uint8_t ds, char *buffer)
- 
- atari/src/fn_fuji/fuji_get_device_filename.s
- 12:; bool fuji_get_device_filename(uint8_t device_slot, char *buffer)
- 
- coco/src/fn_fuji/fuji_get_device_filename.c
- 7:bool fuji_get_device_filename(uint8_t ds, char *buffer)
- 
- commodore/src/fn_fuji/fuji_get_device_filename.c
- 8:bool fuji_get_device_filename(uint8_t ds, char *buffer)
- 
- fujinet-fuji.h
- 344:bool fuji_get_device_filename(uint8_t ds, char *buffer);
- 
- msdos/src/fn_fuji/fuji_get_device_filename.c
- 6:bool fuji_get_device_filename(uint8_t ds, char *buffer)
- 
- pmd85/src/fn_fuji/fuji_get_device_filename.c
- 5:bool fuji_get_device_filename(uint8_t ds, char *buffer)


----

[Back to index](../index.md)
