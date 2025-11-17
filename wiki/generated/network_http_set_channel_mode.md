# network_http_set_channel_mode

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Sets the channel mode.
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  mode The mode to set
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open connection.
 */
uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*devicespec` | `const char` | _TODO: describe parameter_ |
| `mode` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Perform JSON query
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  query pointer to string containing json path to query, e.g. "/path/field". No need to add device drive.
 * @param  s pointer to receiving string, nul terminated, if no data was retrieved, sets it to an empty string
 * @return Bytes read, or negative values represent fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open and parsed json.
 */
```

## Implementations (git grep results)

- adam/src/fn_network/network_http_set_channel_mode.c
- 16:uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode)
- 
- apple2/src/fn_network/network_http_set_channel_mode.c
- 4:uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode) {
- 
- atari/src/fn_network/network_http_set_channel_mode.c
- 4:uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode) {
- 
- coco/src/fn_network/network_http_set_channel_mode.c
- 8:uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode)
- 
- commodore/src/fn_network/network_http_set_channel_mode.c
- 9:uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode)
- 
- common/src/fn_network/network_http_end_add_headers.c
- 9:    return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
- 
- common/src/fn_network/network_http_post.c
- 11:    err = network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_POST_SET_DATA);
- 15:    return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
- 
- common/src/fn_network/network_http_post_bin.c
- 11:    err = network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_POST_SET_DATA);
- 15:    return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_BODY);
- 
- common/src/fn_network/network_http_start_add_headers.c
- 9:    return network_http_set_channel_mode(devicespec, HTTP_CHAN_MODE_SET_HEADERS);
- 
- fujinet-network.h
- 177:uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode);
- 
- msdos/src/fn_network/network_http_set_channel_mode.c
- 5:uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode)
- 
- pmd85/src/fn_network/network_http_set_channel_mode.c
- 6:uint8_t network_http_set_channel_mode(const char *devicespec, uint8_t mode)


----

[Back to index](../index.md)
