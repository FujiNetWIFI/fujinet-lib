# network_fs_unlock

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief Unlock file (make read/write) on FS (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to device specification "N1:TNFS://TMA-2/foo.txt"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
uint8_t network_fs_unlock(const char *devicespec);
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
 * @brief Lock file (make read only) on FS (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to device specification "N1:TNFS://TMA-2/foo.txt"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_fs_unlock.c
- 15:uint8_t network_fs_unlock(const char *devicespec) {
- 
- fujinet-network.h
- 284:uint8_t network_fs_unlock(const char *devicespec);


----

[Back to index](../index.md)
