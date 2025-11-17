# fuji_set_ssid

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Set the SSID information from NetConfig structure
 * @return success status of request
 */
bool fuji_set_ssid(NetConfig *nc);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*nc` | `NetConfig` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Set the host prefix for given host slot id for platforms that support it.
 * @return success status of request
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_set_ssid.c
- 9:bool fuji_set_ssid(NetConfig *nc)
- 
- apple2/src/fn_fuji/fuji_set_ssid.c
- 6:bool fuji_set_ssid(NetConfig *nc)
- 
- atari/src/fn_fuji/fuji_set_ssid.s
- 12:; bool fuji_set_ssid(NetConfig *fuji_net_config)
- 
- coco/src/fn_fuji/fuji_set_ssid.c
- 7:bool fuji_set_ssid(NetConfig *nc)
- 
- commodore/src/fn_fuji/fuji_set_ssid.c
- 8:bool fuji_set_ssid(NetConfig *nc)
- 
- fujinet-fuji.h
- 525:bool fuji_set_ssid(NetConfig *nc);
- 
- msdos/src/fn_fuji/fuji_set_ssid.c
- 6:bool fuji_set_ssid(NetConfig *nc)
- 
- pmd85/src/fn_fuji/fuji_set_ssid.c
- 5:bool fuji_set_ssid(NetConfig *nc)


----

[Back to index](../index.md)
