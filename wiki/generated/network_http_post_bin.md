# network_http_post_bin

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Send POST HTTP request, sends binary data from data location for length len, which allows sending arbitrary binary data
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data binary data to post
 * @param  len length of binary data to send
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
uint8_t network_http_post_bin(const char *devicespec, const uint8_t *data, uint16_t len);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*devicespec` | `const char` | _TODO: describe parameter_ |
| `*data` | `const uint8_t` | _TODO: describe parameter_ |
| `len` | `uint16_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Send POST HTTP request - assumes data is a string with nul terminator. This will not be able to send the 00 byte
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data text data to post
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_http_post_bin.c
- 9:uint8_t network_http_post_bin(const char *devicespec, const uint8_t *data, uint16_t len) {
- 
- fujinet-network.h
- 228:uint8_t network_http_post_bin(const char *devicespec, const uint8_t *data, uint16_t len);


----

[Back to index](../index.md)
