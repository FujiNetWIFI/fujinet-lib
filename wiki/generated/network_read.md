# network_read

**Declared in:** `fujinet-network.h`

## Prototype

```c
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
int16_t network_read(const char* devicespec, uint8_t *buf, uint16_t len);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `devicespec` | `const char*` | _TODO: describe parameter_ |
| `*buf` | `uint8_t` | _TODO: describe parameter_ |
| `len` | `uint16_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `int16_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Non-blocking read from channel
 * 
 * The read will grab whatever is waiting in the FujiNet buffer. If fewer than the requested len, the return count will reflect this.
 * Errors are returned as the negative value of the FUJI standard error. fn_network_error contains the device specific error code. fn_bytes_read will be 0 on errors.
 * 
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return Bytes read, or negative value of fujinet-network error code (See FN_ERR_* values) with fn_network_error containing real error code
 */
```

## Implementations (git grep results)

- coco/src/fn_network/network_json_query.c
- 38:    return network_read(devicespec, (uint8_t *)s, bw);
- 
- common/src/fn_network/network_read.c
- 50:int16_t network_read(const char *devicespec, uint8_t *buf, uint16_t len)
- 
- fujinet-network.h
- 40: * The number of bytes read in the last call to network_read().
- 127:int16_t network_read(const char* devicespec, uint8_t *buf, uint16_t len);
- 
- msdos/src/fn_network/network_json_query.c
- 24:  return network_read(devicespec,(uint8_t *)s, bw);
- 
- pmd85/src/fn_network/network_json_query.c
- 37:    return network_read(devicespec, (uint8_t *)s, bw);


----

[Back to index](../index.md)
