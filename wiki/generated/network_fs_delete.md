# network_fs_delete

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief Delete file from FS endpoint (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to device specification e.g. "N1:TNFS://TMA-2/foo.txt"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
uint8_t network_fs_delete(const char *devicespec);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*devicespec` | `const char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Internal routine to get the network UNIT id from the devicespec, i.e. Nx: find the "x" value
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return unit number
 * 
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_fs_delete.c
- 15:uint8_t network_fs_delete(const char *devicespec) {
- 
- fujinet-network.h
- 263:uint8_t network_fs_delete(const char *devicespec);


----

[Back to index](../index.md)
