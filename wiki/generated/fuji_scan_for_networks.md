# fuji_scan_for_networks

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Scans network for SSIDs and sets count accordingly.
 * @return success status of request.
 */
bool fuji_scan_for_networks(uint8_t *count);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*count` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Reset FN
 * @return true if successful, false if there was an error from FN
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_scan_for_networks.c
- 9:bool fuji_scan_for_networks(uint8_t *count)
- 
- apple2/src/fn_fuji/fuji_scan_for_networks.c
- 5:bool fuji_scan_for_networks(uint8_t *count)
- 
- atari/src/fn_fuji/fuji_scan_for_networks.s
- 11:; bool fuji_scan_for_networks(uint8_t *count)
- 
- coco/src/fn_fuji/fuji_scan_for_networks.c
- 7:bool fuji_scan_for_networks(uint8_t *count)
- 
- commodore/src/fn_fuji/fuji_scan_for_networks.c
- 6:bool fuji_scan_for_networks(uint8_t *count)
- 
- fujinet-fuji.h
- 468:bool fuji_scan_for_networks(uint8_t *count);
- 
- msdos/src/fn_fuji/fuji_scan_for_networks.c
- 6:bool fuji_scan_for_networks(uint8_t *count)
- 
- pmd85/src/fn_fuji/fuji_scan_for_networks.c
- 5:bool fuji_scan_for_networks(uint8_t *count)


----

[Back to index](../index.md)
