# network_write

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Write to network 
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_write(const char* devicespec, const uint8_t *buf, uint16_t len);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `devicespec` | `const char*` | _TODO: describe parameter_ |
| `*buf` | `const uint8_t` | _TODO: describe parameter_ |
| `len` | `uint16_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Read from channel
 * 
 * The read will block until it has read all the bytes requested from the device, or the EOF is hit.
 * This will block waiting for as much data as it can, so that the client does not need to handle counting.
 * Errors are returned as the negative value of the error. fn_network_error contains the device specific error code. fn_bytes_read will contain the count of bytes read before error occurred.
 * 
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return Bytes read, or negative value of fujinet-network error code (See FN_ERR_* values) with fn_network_error containing real error code
 */
```

## Implementations (git grep results)

- adam/src/fn_network/network_write.c
- 19:uint8_t network_write(const char *devicespec, const uint8_t *buf, uint16_t len)
- 
- apple2/src/fn_network/network_write.c
- 12:uint8_t network_write(const char *devicespec, const uint8_t *buf, uint16_t len) {
- 
- atari/src/fn_network/network_write.s
- 14:; uint8_t network_write(const char *devicespec, const uint8_t *buf, uint16_t len)
- 
- coco/src/fn_network/network_write.c
- 8:uint8_t network_write(const char* devicespec, const uint8_t *buf, uint16_t len)
- 
- commodore/src/fn_network/network_write.c
- 6:uint8_t network_write(const char *devicespec, const uint8_t *buf, uint16_t len)
- 
- common/src/fn_network/network_http_add_header.c
- 10:    return network_write(devicespec, (uint8_t *) header, strlen(header));
- 
- common/src/fn_network/network_http_post.c
- 13:    err = network_write(devicespec, (uint8_t *) data, strlen(data));
- 
- common/src/fn_network/network_http_post_bin.c
- 13:    err = network_write(devicespec, data, len);
- 
- fujinet-network.h
- 136:uint8_t network_write(const char* devicespec, const uint8_t *buf, uint16_t len);
- 
- msdos/src/fn_network/network_write.c
- 6:uint8_t network_write(const char* devicespec, const uint8_t *buf, uint16_t len)
- 
- pmd85/src/fn_network/network_write.c
- 6:uint8_t network_write(const char* devicespec, const uint8_t *buf, uint16_t len)


----

[Back to index](../index.md)
