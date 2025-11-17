# fuji_status

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Gets the FNStatus information from FUJI device.
 * @return success status of the status request
 * NOTE: The actual status VALUE is in 'status', the return is just whether the command to fetch the status succeeded, it could succeed, but the status value holds an error.
 */
bool fuji_status(FNStatus *status);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*status` | `FNStatus` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Set the SSID information from NetConfig structure
 * @return success status of request
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_status.c
- 5:bool fuji_status(FNStatus *status)
- 
- apple2/src/fn_fuji/fuji_status.c
- 6:bool fuji_status(FNStatus *status)
- 
- atari/src/fn_fuji/fuji_status.s
- 11:; bool fuji_status(FNStatus *status);
- 
- coco/src/fn_fuji/fuji_status.c
- 7:bool fuji_status(FNStatus *status)
- 
- commodore/src/fn_fuji/fuji_status.c
- 8:bool fuji_status(FNStatus *status)
- 
- commodore/src/fn_fuji/get_fuji_status.c
- 13:bool get_fuji_status(bool should_close)
- 
- commodore/src/fn_fuji/open_close.c
- 15:	return get_fuji_status(true);
- 
- commodore/src/fn_fuji/open_close_data.c
- 23:	is_success = get_fuji_status(should_close);
- 
- commodore/src/fn_fuji/open_read_close.c
- 47:	return get_fuji_status(should_close);
- 
- commodore/src/fn_fuji/open_read_close_data.c
- 30:	is_success = get_fuji_status(should_close);
- 
- commodore/src/include/fujinet-fuji-cbm.h
- 33:bool get_fuji_status(bool should_close);
- 
- fujinet-fuji.h
- 532:bool fuji_status(FNStatus *status);
- 
- msdos/src/fn_fuji/fuji_status.c
- 6:bool fuji_status(FNStatus *status)
- 
- pmd85/src/fn_fuji/fuji_status.c
- 5:bool fuji_status(FNStatus *status)


----

[Back to index](../index.md)
