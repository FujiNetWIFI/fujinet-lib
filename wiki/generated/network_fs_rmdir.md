# network_fs_rmdir

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief Remove directory on FS (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec pointer to devicespec "N1:TNFS://TMA-2/newdir"
 * @return fujinet-network error code (see FN_ERR_* values)
 * @verbose Directory must be empty!
 */
uint8_t network_fs_rmdir(const char *devicespec);
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
 * @brief Make directory on FS (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec pointer to devicespec "N1:TNFS://TMA-2/newdir"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_fs_rmdir.c
- 16:uint8_t network_fs_rmdir(const char *devicespec) {
- 
- fujinet-network.h
- 299:uint8_t network_fs_rmdir(const char *devicespec);


----

[Back to index](../index.md)
