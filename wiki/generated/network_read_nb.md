# network_read_nb

**Declared in:** `fujinet-network.h`

## Prototype

```c
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
int16_t network_read_nb(const char* devicespec, uint8_t *buf, uint16_t len);
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
 * @brief  Open Connection
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param  mode (4=read, 8=write, 12=read/write, 13=POST, etc.)
 * @param  trans translation mode (CR/LF to other line endings; 0=none, 1=CR, 2=LF, 3=CRLF, 4=Pet)
 * @return fujinet-network error code (See FN_ERR_* values)
 */
```

## Implementations (git grep results)

- Changelog.md
- 59:- [pmd85] forgotten network_read_nb() [Jan Krupa]
- 
- common/src/fn_network/network_read_nb.c
- 46:int16_t network_read_nb(const char *devicespec, uint8_t *buf, uint16_t len)
- 
- fujinet-network.h
- 113:int16_t network_read_nb(const char* devicespec, uint8_t *buf, uint16_t len);


----

[Back to index](../index.md)
