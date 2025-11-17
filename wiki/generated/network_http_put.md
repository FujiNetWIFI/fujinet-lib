# network_http_put

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Send PUT HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data data to put
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
uint8_t network_http_put(const char *devicespec, const char *data);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*devicespec` | `const char` | _TODO: describe parameter_ |
| `*data` | `const char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Send POST HTTP request, sends binary data from data location for length len, which allows sending arbitrary binary data
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data binary data to post
 * @param  len length of binary data to send
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_http_put.c
- 8:uint8_t network_http_put(const char *devicespec, const char *data) {
- 
- fujinet-network.h
- 238:uint8_t network_http_put(const char *devicespec, const char *data);


----

[Back to index](../index.md)
