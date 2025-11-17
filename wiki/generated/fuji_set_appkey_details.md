# fuji_set_appkey_details

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief  Sets the base details for appkeys. This must be called before using read or write operations on appkeys.
 * @param  creator_id the id of the creator of the appkey
 * @param  app_id the id of the application from the creator
 * @param  keysize type: AppKeySize, set to DEFAULT for 64 byte appkeys, or SIZE_256 for 256 byte keys
 */
void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `creator_id` | `uint16_t` | _TODO: describe parameter_ |
| `app_id` | `uint8_t` | _TODO: describe parameter_ |
| `keysize` | `enum AppKeySize` | _TODO: describe parameter_ |

## Return Value

- **Type:** `void`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Writes to an appkey using the provided details previously setup with fuji_set_appkey_details.
 * @param  key_id the specific key id of this application.
 * @param  count the number of bytes in the buffer to write to the appkey.
 * @param  data a pointer to the memory to write from.
 * @return success status of the call. If either the initial OPEN or subsequent WRITE fail, will return false.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_set_appkey_details.c
- 8:void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize)
- 
- common/src/fn_fuji/fuji_set_appkey_details.c
- 17:void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize)
- 
- fujinet-fuji.h
- 575:void fuji_set_appkey_details(uint16_t creator_id, uint8_t app_id, enum AppKeySize keysize);


----

[Back to index](../index.md)
