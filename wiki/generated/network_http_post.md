# network_http_post

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Send POST HTTP request - assumes data is a string with nul terminator. This will not be able to send the 00 byte
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data text data to post
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
uint8_t network_http_post(const char *devicespec, const char *data);
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
 * @brief  Add header to HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  header pointer to string containing full header to add, e.g. "Accept: application/json"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_http_post.c
- 9:uint8_t network_http_post(const char *devicespec, const char *data) {
- 
- common/src/fn_network/network_http_put.c
- 9:    return network_http_post(devicespec, data);
- 
- fujinet-network.h
- 216:uint8_t network_http_post(const char *devicespec, const char *data);


----

[Back to index](../index.md)
