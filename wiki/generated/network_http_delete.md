# network_http_delete

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Send DELETE HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  trans translation value
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * This will open a connection, consumer can then query the data, and must close the connection.
 */
uint8_t network_http_delete(const char *devicespec, uint8_t trans);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*devicespec` | `const char` | _TODO: describe parameter_ |
| `trans` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Send PUT HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  data data to put
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_http_delete.c
- 8:uint8_t network_http_delete(const char *devicespec, uint8_t trans) {
- 
- fujinet-network.h
- 248:uint8_t network_http_delete(const char *devicespec, uint8_t trans);


----

[Back to index](../index.md)
