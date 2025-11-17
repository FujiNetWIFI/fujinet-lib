# network_status

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Get Network Device Status byte
 * @param  devicespec pointer to device specification of form: N:PROTO://[HOSTNAME]:PORT/PATH/.../
 * @param  bw pointer to where to put bytes waiting
 * @param  c pointer to where to put connection status
 * @param  err to where to put network error byte.
 * @return fujinet-network status/error code (See FN_ERR_* values)
 */
uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*devicespec` | `const char` | _TODO: describe parameter_ |
| `*bw` | `uint16_t` | _TODO: describe parameter_ |
| `*c` | `uint8_t` | _TODO: describe parameter_ |
| `*err` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Initialise network device
 * Allows initialisation of network to perform any platform dependent checks, and allow applications to
 * exit early if there is a network issue.
 * @return fujinet-network status/error code (See FN_ERR_* values) and set device specific error if there is any
 */
```

## Implementations (git grep results)

- adam/src/fn_network/network_status.c
- 20:uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
- 
- apple2/src/fn_network/network_status.c
- 9:uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err) {
- 
- atari/src/fn_network/network_status.s
- 17:; uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
- 
- coco/src/fn_network/network_json_query.c
- 33:    network_status(devicespec, &bw, &c, &err);
- 
- coco/src/fn_network/network_status.c
- 8:uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
- 
- commodore/src/fn_network/network_json_query.c
- 45:	status_code = network_status(devicespec, &bw, &conn, &err);
- 
- commodore/src/fn_network/network_status.c
- 9:uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
- 
- common/src/fn_network/network_read.c
- 114:        r = network_status(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error);
- 116:        r = network_status(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error);
- 118:        r = network_status(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error);
- 
- common/src/fn_network/network_read_nb.c
- 100:    r = network_status(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error);
- 102:    r = network_status(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error);
- 104:    r = network_status(devicespec, &fn_network_bw, &fn_network_conn, &fn_network_error); // TODO: Status return needs fixing.
- 
- fujinet-network.h
- 84:uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err);
- 
- msdos/src/fn_network/network_json_query.c
- 19:  network_status(devicespec,&bw,&c,&err);
- 
- msdos/src/fn_network/network_status.c
- 6:uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)
- 
- pmd85/src/fn_network/network_json_query.c
- 32:    network_status(devicespec, &bw, &c, &err);
- 
- pmd85/src/fn_network/network_status.c
- 6:uint8_t network_status(const char *devicespec, uint16_t *bw, uint8_t *c, uint8_t *err)


----

[Back to index](../index.md)
