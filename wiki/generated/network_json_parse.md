# network_json_parse

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Parse the currently open JSON location
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * This will set the channel mode to JSON, which will be unset in the close.
 */
uint8_t network_json_parse(const char *devicespec);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*devicespec` | `const char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Device specific direct control commands
 * @param  cmd Command byte to send
 * @param  aux1 Auxiliary byte 1
 * @param  aux2 Auxiliary byte 2
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  ... varargs - Device specific additional parameters to pass to the network device
 * @return fujinet-network error code (See FN_ERR_* values)
 */
```

## Implementations (git grep results)

- adam/src/fn_network/network_json_parse.c
- 15:uint8_t network_json_parse(const char *devicespec)
- 
- apple2/src/fn_network/network_json_parse.c
- 8:uint8_t network_json_parse(const char *devicespec) {
- 
- atari/src/fn_network/network_json_parse.s
- 14:; uint8_t network_json_parse(const char *devicespec);
- 
- coco/src/fn_network/network_json_parse.c
- 8:uint8_t network_json_parse(const char *devicespec)
- 
- commodore/src/fn_network/network_json_parse.c
- 9:uint8_t network_json_parse(const char *devicespec)
- 
- fujinet-network.h
- 156:uint8_t network_json_parse(const char *devicespec);
- 
- msdos/src/fn_network/network_json_parse.c
- 6:uint8_t network_json_parse(const char *devicespec)
- 
- pmd85/src/fn_network/network_json_parse.c
- 6:uint8_t network_json_parse(const char *devicespec)


----

[Back to index](../index.md)
