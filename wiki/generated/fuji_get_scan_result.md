# fuji_get_scan_result

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Fills ssid_info with wifi scan results for bssid index n.
 * No data copied if there is an error.
 * @return Success status, true if all OK.
 */
bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `n` | `uint8_t` | _TODO: describe parameter_ |
| `*ssid_info` | `SSIDInfo` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Sets ALL host slot information into pointer h.
 * `size` is the number of host slots, and the returned data size is checked against this multiple of HostSlot structs before copying.
 * If it doesn't match, no data is copied, and false is returned.
 * @return Success status, true if all OK.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_scan_result.c
- 10:bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
- 
- apple2/src/fn_fuji/fuji_get_scan_result.c
- 6:bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
- 
- atari/src/fn_fuji/fuji_get_scan_result.s
- 13:; bool fuji_get_scan_result(uint8_t network_index, SSIDInfo *ssid_info)
- 
- coco/src/fn_fuji/fuji_get_scan_result.c
- 7:bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
- 
- commodore/src/fn_fuji/fuji_get_scan_result.c
- 6:bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
- 
- fujinet-fuji.h
- 379:bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info);
- 
- msdos/src/fn_fuji/fuji_get_scan_result.c
- 6:bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)
- 
- pmd85/src/fn_fuji/fuji_get_scan_result.c
- 5:bool fuji_get_scan_result(uint8_t n, SSIDInfo *ssid_info)


----

[Back to index](../index.md)
