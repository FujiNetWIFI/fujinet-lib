# network_ioctl

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Device specific direct control commands
 * @param  cmd Command byte to send
 * @param  aux1 Auxiliary byte 1
 * @param  aux2 Auxiliary byte 2
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  ... varargs - Device specific additional parameters to pass to the network device
 * @return fujinet-network error code (See FN_ERR_* values)
 */
uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `cmd` | `uint8_t` | _TODO: describe parameter_ |
| `aux1` | `uint8_t` | _TODO: describe parameter_ |
| `aux2` | `uint8_t` | _TODO: describe parameter_ |
| `devicespec` | `const char*` | _TODO: describe parameter_ |
| `` | `...` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Write to network 
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  buf Buffer
 * @param  len length
 * @return fujinet-network error code (See FN_ERR_* values)
 */
```

## Implementations (git grep results)

- adam/src/fn_network/network_ioctl.c
- 5:uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...)
- 
- apple2/src/fn_network/network_http_set_channel_mode.c
- 5:    return network_ioctl('M', 0, mode, devicespec, 1, 0, 2);
- 
- apple2/src/fn_network/network_ioctl.c
- 11:// uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, int16_t use_aux, void *buffer, uint16_t len);
- 18:uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...) {
- 
- atari/src/fn_network/network_http_set_channel_mode.c
- 5:    return network_ioctl('M', 0, mode, devicespec, 0, 0, 0);
- 
- atari/src/fn_network/network_ioctl.s
- 16:; uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...);
- 19:; uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, [uint8_t dstats, uint16_t dbuf, uint16_t dbyt]);
- 
- coco/src/fn_network/network_ioctl.c
- 8:uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...)
- 
- commodore/src/fn_network/network_ioctl.c
- 5:uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...)
- 
- common/src/fn_network/network_fs_cd.c
- 17:    return network_ioctl(',',0,0,devicespec);
- 
- common/src/fn_network/network_fs_delete.c
- 16:    return network_ioctl('!',0,0,devicespec);
- 
- common/src/fn_network/network_fs_lock.c
- 16:    return network_ioctl('#',0,0,devicespec);
- 
- common/src/fn_network/network_fs_mkdir.c
- 16:    return network_ioctl('*',0,0,devicespec);
- 
- common/src/fn_network/network_fs_pwd.c
- 18:    return network_ioctl('0',0,0,devicespec);
- 
- common/src/fn_network/network_fs_rename.c
- 17:    return network_ioctl(' ',0,0,devicespec);
- 
- common/src/fn_network/network_fs_rmdir.c
- 17:    return network_ioctl('+',0,0,devicespec);
- 
- common/src/fn_network/network_fs_unlock.c
- 16:    return network_ioctl('$',0,0,devicespec);
- 
- fujinet-network.h
- 147:uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...);
- 
- msdos/src/fn_network/network_ioctl.c
- 9:uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...)
- 
- pmd85/src/fn_network/network_ioctl.c
- 7:uint8_t network_ioctl(uint8_t cmd, uint8_t aux1, uint8_t aux2, const char* devicespec, ...)


----

[Back to index](../index.md)
