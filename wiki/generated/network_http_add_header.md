# network_http_add_header

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Add header to HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  header pointer to string containing full header to add, e.g. "Accept: application/json"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
uint8_t network_http_add_header(const char *devicespec, const char *header);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*devicespec` | `const char` | _TODO: describe parameter_ |
| `*header` | `const char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  End adding headers.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection. Completes header adding, and sets mode back to BODY
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_http_add_header.c
- 9:uint8_t network_http_add_header(const char *devicespec, const char *header) {
- 
- fujinet-network.h
- 205:uint8_t network_http_add_header(const char *devicespec, const char *header);


----

[Back to index](../index.md)
