# network_close

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Close Connection
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_close(const char* devicespec);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `devicespec` | `const char*` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Get Network Device Status byte
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param  bw pointer to where to put bytes waiting
 * @param  c pointer to where to put connection status
 * @param  err to where to put network error byte.
 * @return fujinet-network status/error code (See FN_ERR_* values)
 */
```

## Implementations (git grep results)

- adam/src/fn_network/network_close.c
- 5:uint8_t network_close(const char* devicespec)
- 
- apple2/apple2-6502/fn_network/network_close.s
- 13:; uint8_t network_close(const char* devicespec);
- 
- atari/src/fn_network/network_close.s
- 14:; uint8_t network_close(const char* devicespec)
- 
- coco/src/fn_network/network_close.c
- 8:uint8_t network_close(const char* devicespec)
- 
- commodore/src/fn_network/network_close.c
- 6:uint8_t network_close(const char* devicespec)
- 
- fujinet-network.h
- 91:uint8_t network_close(const char* devicespec);
- 
- msdos/src/fn_network/network_close.c
- 6:uint8_t network_close(const char* devicespec)
- 
- pmd85/src/fn_network/network_close.c
- 6:uint8_t network_close(const char* devicespec)


----

[Back to index](../index.md)
