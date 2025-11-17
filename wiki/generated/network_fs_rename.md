# network_fs_rename

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief Rename file on FS endpoint (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to device specification, with new name after comma, e.g. "N1:TNFS://TMA-2/foo.txt,bar.txt"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
uint8_t network_fs_rename(const char *devicespec);
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
 * @brief Delete file from FS endpoint (e.g. TNFS, FTP, HTTPS, SMB)
 * @param devicespec Pointer to device specification e.g. "N1:TNFS://TMA-2/foo.txt"
 * @return fujinet-network error code (see FN_ERR_* values)
 */
```

## Implementations (git grep results)

- common/src/fn_network/network_fs_rename.c
- 16:uint8_t network_fs_rename(const char *devicespec) {
- 
- fujinet-network.h
- 270:uint8_t network_fs_rename(const char *devicespec);


----

[Back to index](../index.md)
