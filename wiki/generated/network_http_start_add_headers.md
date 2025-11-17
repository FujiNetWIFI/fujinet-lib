# network_http_start_add_headers

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Start adding headers.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection. After calling this, add any headers with network_http_add_header, and finally call network_http_end_add_headers
 */
uint8_t network_http_start_add_headers(const char *devicespec);
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
 * @brief  Sets the channel mode.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  mode The mode to set
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_http_start_add_headers.c
- 8:uint8_t network_http_start_add_headers(const char *devicespec) {
- 
- fujinet-network.h
- 186:uint8_t network_http_start_add_headers(const char *devicespec);


----

[Back to index](../index.md)
