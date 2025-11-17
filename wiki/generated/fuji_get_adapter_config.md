# fuji_get_adapter_config

**Declared in:** `fujinet-fuji.h`

## Prototype

```c
/**
 * @brief Gets adapter config information from FN, e.g. IP, MAC, BSSID etc.
 * Raw version that returns bytes for all IP etc related values.
 * @return Success status, true if all OK.
 */
bool fuji_get_adapter_config(AdapterConfig *ac);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*ac` | `AdapterConfig` | _TODO: describe parameter_ |

## Return Value

- **Type:** `bool`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief Returns true if last operation had an error.
 * @return ERROR status, true if there was an error in last operation.
 */
```

## Implementations (git grep results)

- adam/src/fn_fuji/fuji_get_adapter_config.c
- 10:bool fuji_get_adapter_config(AdapterConfig *ac)
- 
- apple2/src/fn_fuji/fuji_get_adapter_config.c
- 7:bool fuji_get_adapter_config(AdapterConfig *ac)
- 
- atari/src/fn_fuji/fuji_get_adapter_config.s
- 13:; bool fuji_get_adapter_config(void *adapter_config)
- 
- coco/src/fn_fuji/fuji_get_adapter_config.c
- 7:bool fuji_get_adapter_config(AdapterConfig *ac)
- 
- commodore/src/fn_fuji/fuji_get_adapter_config.c
- 6:bool fuji_get_adapter_config(AdapterConfig *ac)
- 
- fujinet-fuji.h
- 322:bool fuji_get_adapter_config(AdapterConfig *ac);
- 
- msdos/src/fn_fuji/fuji_get_adapter_config.c
- 6:bool fuji_get_adapter_config(AdapterConfig *ac)
- 
- pmd85/src/fn_fuji/fuji_get_adapter_config.c
- 5:bool fuji_get_adapter_config(AdapterConfig *ac)


----

[Back to index](../index.md)
