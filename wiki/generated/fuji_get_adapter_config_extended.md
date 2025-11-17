# fuji_get_adapter_config_extended

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Gets extended adapter config information from FN, e.g. IP, MAC, BSSID etc.
 * Extended version that returns strings in addition to raw for all IP etc related values.
 * @return Success status, true if all OK.
 */
bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*ac` | `AdapterConfigExtended` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Gets adapter config information from FN, e.g. IP, MAC, BSSID etc.
 * Raw version that returns bytes for all IP etc related values.
 * @return Success status, true if all OK.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_adapter_config_extended.c
- 10:bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
- 
- apple2/src/fn_fuji/fuji_get_adapter_config_extended.c
- 7:bool fuji_get_adapter_config_extended(AdapterConfigExtended *acx)
- 
- atari/src/fn_fuji/fuji_get_adapter_config_extended.s
- 12:; bool fuji_get_adapter_config_extended(AdapterConfigExtended *adapter_config)
- 
- coco/src/fn_fuji/fuji_get_adapter_config_extended.c
- 7:bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
- 
- commodore/src/fn_fuji/fuji_get_adapter_config_extended.c
- 6:bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
- 
- fujinet-fuji.h
- 329:bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac);
- 
- msdos/src/fn_fuji/fuji_get_adapter_config_extended.c
- 6:bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)
- 
- pmd85/src/fn_fuji/fuji_get_adapter_config_extended.c
- 5:bool fuji_get_adapter_config_extended(AdapterConfigExtended *ac)


----

[Back to index](../index.md)
