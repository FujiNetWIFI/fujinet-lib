# network_open

**Declared in:** `fujinet-network.h`

## Prototype

```c
0=none, 1=CR, 2=LF, 3=CRLF, 4=Pet)
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `devicespec` | `const char*` | _TODO: describe parameter_ |
| `mode` | `uint8_t` | _TODO: describe parameter_ |
| `trans` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `0=none, 1=CR, 2=LF, 3=CRLF, 4=Pet) * @return fujinet-network error code (See FN_ERR_* values) */ uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Close Connection
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @return fujinet-network error code (See FN_ERR_* values)
 */
```

## Implementations (git grep results)

- adam/src/fn_network/network_open.c
- 25:uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans)
- 
- apple2/src/fn_network/network_open.c
- 6:uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans) {
- 
- atari/src/fn_network/network_open.s
- 17:; uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans);
- 
- coco/src/fn_network/network_open.c
- 8:uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans)
- 
- commodore/src/fn_network/network_open.c
- 10:uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans)
- 
- common/src/fn_network/network_http_delete.c
- 9:    return network_open(devicespec, OPEN_MODE_HTTP_DELETE_H, trans);
- 
- fujinet-network.h
- 100:uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans);
- 
- msdos/src/fn_network/network_open.c
- 6:uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans)
- 
- pmd85/src/fn_network/network_open.c
- 7:uint8_t network_open(const char* devicespec, uint8_t mode, uint8_t trans)


----

[Back to index](../index.md)
