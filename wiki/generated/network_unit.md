# network_unit

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Internal routine to get the network UNIT id from the devicespec, i.e. Nx: find the "x" value
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return unit number
 * 
 */
uint8_t network_unit(const char *devicespec);
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
 * @brief  Send DELETE HTTP request
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  trans translation value
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * This will open a connection, consumer can then query the data, and must close the connection.
 */
```

## Implementations (git grep results)

- adam/src/fn_network/network_unit_adamnet.c
- 18:    if (!(u = network_unit(devicespec)))
- 
- apple2/src/fn_network/network_ioctl.c
- 54:    sp_nw_unit = network_unit(devicespec);
- 
- apple2/src/fn_network/network_json_parse.c
- 17:	sp_nw_unit = network_unit(devicespec);
- 
- apple2/src/fn_network/network_json_query.c
- 21:	sp_nw_unit = network_unit(devicespec);
- 
- apple2/src/fn_network/network_open.c
- 28:	sp_nw_unit = network_unit(devicespec);
- 
- apple2/src/fn_network/network_status.c
- 17:	sp_nw_unit = network_unit(devicespec);
- 
- apple2/src/fn_network/network_write.c
- 24:	sp_nw_unit = network_unit(devicespec);
- 
- coco/src/fn_network/network_close.c
- 19:    nc.unit = network_unit(devicespec);
- 
- coco/src/fn_network/network_http_set_channel_mode.c
- 21:    hscm.unit = network_unit(devicespec);
- 
- coco/src/fn_network/network_ioctl.c
- 21:    ioctl.unit = network_unit(devicespec);
- 
- coco/src/fn_network/network_json_parse.c
- 20:    jp.unit = network_unit(devicespec);
- 
- coco/src/fn_network/network_json_query.c
- 22:    uint8_t unit = network_unit(devicespec);
- 
- coco/src/fn_network/network_open.c
- 21:    o.unit = network_unit(devicespec);
- 
- coco/src/fn_network/network_read_coco.c
- 20:    uint8_t unit = network_unit(devicespec);
- 
- coco/src/fn_network/network_status.c
- 27:    uint8_t unit = network_unit(devicespec);
- 
- coco/src/fn_network/network_write.c
- 19:    w.unit = network_unit(devicespec);
- 
- coco/src/include/fujinet-network-coco.h
- 55:uint8_t network_unit(const char *devicespec);
- 
- common/src/fn_network/network_read.c
- 100:    unit = network_unit(devicespec);
- 
- common/src/fn_network/network_read_nb.c
- 92:    unit = network_unit(devicespec);
- 
- common/src/fn_network/network_unit.c
- 18:uint8_t network_unit(const char *devicespec)
- 
- fujinet-network.h
- 256:uint8_t network_unit(const char *devicespec);
- 
- msdos/src/fn_network/network_close.c
- 8:  uint8_t device = network_unit(devicespec) + 0x70;
- 
- msdos/src/fn_network/network_http_set_channel_mode.c
- 7:  uint8_t device = network_unit(devicespec) + 0x70;
- 
- msdos/src/fn_network/network_ioctl.c
- 11:  uint8_t device = network_unit(devicespec) + 0x70;
- 
- msdos/src/fn_network/network_json_parse.c
- 8:  uint8_t device = network_unit(devicespec) + 0x70;
- 
- msdos/src/fn_network/network_json_query.c
- 8:  uint8_t device = network_unit(devicespec) + 0x70;
- 
- msdos/src/fn_network/network_open.c
- 8:  uint8_t device = network_unit(devicespec) + 0x70;
- 
- msdos/src/fn_network/network_read_msdos.c
- 8:  uint8_t device = network_unit(devicespec) + 0x70;
- 
- msdos/src/fn_network/network_status.c
- 8:  uint8_t device = network_unit(devicespec) + 0x70;
- 
- msdos/src/fn_network/network_write.c
- 8:  uint8_t device = network_unit(devicespec) + 0x70;
- 
- pmd85/src/fn_network/network_close.c
- 17:    nc.unit = network_unit(devicespec);
- 
- pmd85/src/fn_network/network_http_set_channel_mode.c
- 19:    hscm.unit = network_unit(devicespec);
- 
- pmd85/src/fn_network/network_ioctl.c
- 20:    ioctl.unit = network_unit(devicespec);
- 
- pmd85/src/fn_network/network_json_parse.c
- 18:    jp.unit = network_unit(devicespec);
- 
- pmd85/src/fn_network/network_json_query.c
- 21:    uint8_t unit = network_unit(devicespec);
- 
- pmd85/src/fn_network/network_open.c
- 20:    o.unit = network_unit(devicespec);
- 
- pmd85/src/fn_network/network_read_pmd85.c
- 18:    uint8_t unit = network_unit(devicespec);
- 
- pmd85/src/fn_network/network_status.c
- 25:    uint8_t unit = network_unit(devicespec);
- 
- pmd85/src/fn_network/network_write.c
- 17:    w.unit = network_unit(devicespec);
- 
- pmd85/src/include/fujinet-network-pmd85.h
- 57:uint8_t network_unit(const char *devicespec);


----

[Back to index](../index.md)
