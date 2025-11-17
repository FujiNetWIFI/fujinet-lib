# fuji_get_ssid

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Fills net_config with current SSID/password values.
 * No data copied if there is an error.
 * @return Success status, true if all OK.
 */
bool fuji_get_ssid(NetConfig *net_config);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*net_config` | `NetConfig` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Fills ssid_info with wifi scan results for bssid index n.
 * No data copied if there is an error.
 * @return Success status, true if all OK.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_ssid.c
- 10:bool fuji_get_ssid(NetConfig *net_config)
- 
- apple2/src/fn_fuji/fuji_get_ssid.c
- 6:bool fuji_get_ssid(NetConfig *net_config)
- 
- atari/src/fn_fuji/fuji_get_ssid.s
- 12:; bool _fuji_get_ssid(NetConfig *net_config)
- 
- coco/src/fn_fuji/fuji_get_ssid.c
- 7:bool fuji_get_ssid(NetConfig *net_config)
- 
- commodore/src/fn_fuji/fuji_get_ssid.c
- 6:bool fuji_get_ssid(NetConfig *net_config)
- 
- fujinet-fuji.h
- 386:bool fuji_get_ssid(NetConfig *net_config);
- 
- msdos/src/fn_fuji/fuji_get_ssid.c
- 6:bool fuji_get_ssid(NetConfig *net_config)
- 
- pmd85/src/fn_fuji/fuji_get_ssid.c
- 5:bool fuji_get_ssid(NetConfig *net_config)


----

[Back to index](../index.md)
