# fn_error

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * Convert device specific error in code to FujiNet Network library error, agnostic of device.
 * Library code calls this when it encounters an error to return value applications should use.
 */
uint8_t fn_error(uint8_t code);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `code` | `uint8_t` | _TODO: describe parameter_ |

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * Device specific error. This is the raw code from any device errors before they are converted to
 * simpler device-agnostic network library errors.
 */
```

## Implementations (git grep results)

- adam/src/fn_network/fn_error.c
- 5:uint8_t fn_error(uint8_t code)
- 
- apple2/apple2-6502/fn_network/fn_error.s
- 9:; uint8_t fn_error(uint8_t code)
- 
- apple2/src/fn_network/network_ioctl.c
- 31:        return fn_error(SP_ERR_BAD_CMD);
- 37:            return fn_error(SP_ERR_IO_ERROR);
- 72:    return fn_error(sp_control_nw(sp_network, cmd));
- 
- apple2/src/fn_network/network_json_parse.c
- 23:		return fn_error(err);
- 27:	return fn_error(sp_control_nw(sp_network, 'P'));
- 
- apple2/src/fn_network/network_json_query.c
- 39:		// no data, set string to null char, and return 0 length. fn_error(0) returns 0, so this saves some bytes
- 59:	return -fn_error(err);
- 
- apple2/src/fn_network/network_open.c
- 12:	fn_error(0);
- 17:            return fn_error(SP_ERR_IO_ERROR);
- 23:	// 	return fn_error(SP_ERR_IO_ERROR);
- 
- apple2/src/fn_network/network_status.c
- 25:	return fn_error(err_status);
- 
- apple2/src/fn_network/network_write.c
- 21:		return fn_error(SP_ERR_BAD_CMD);		
- 35:			return fn_error(err);
- 42:	return fn_error(SP_ERR_OK);
- 
- atari/src/bus/fn_error.s
- 8:; uint8_t fn_error(uint8_t code)
- 
- coco/src/bus/fuji_get_error.c
- 31:    return fn_error(dwread((byte *)&err, 1) ? err : BUS_ERROR) != FN_ERR_OK;
- 
- coco/src/bus/fuji_get_response.c
- 33:    return !fn_error(dwread((byte *)buf, len) ? BUS_SUCCESS : BUS_ERROR);
- 
- coco/src/bus/network_get_error.c
- 39:    return fn_error(dwread((byte *)&err, 1) ? err : BUS_ERROR);
- 
- coco/src/bus/network_get_response.c
- 39:    return fn_error(dwread((byte *)buf, len) ? BUS_SUCCESS : BUS_ERROR);
- 
- coco/src/fn_network/fn_error.c
- 6:uint8_t fn_error(uint8_t code)
- 
- commodore/src/fn_network/fn_error.c
- 5:uint8_t fn_error(uint8_t code)
- 
- commodore/src/fn_network/network_open.c
- 26:			// TODO: this should call fn_error() with the return value from the open call
- 49:		// TODO: fix status, this should call fn_error(<value from cbm_open>)
- 
- common/src/fn_network/network_read.c
- 78:        return fn_error(132); // invalid command
- 80:        return fn_error(SP_ERR_BAD_CMD);
- 84:        return fn_error(132); // invalid command
- 92:        return fn_error(SP_ERR_BAD_UNIT);
- 
- common/src/fn_network/network_read_nb.c
- 71:        return -fn_error(132); // invalid command
- 73:        return -fn_error(SP_ERR_BAD_CMD);
- 77:        return -fn_error(132); // invalid command
- 84:        return -fn_error(SP_ERR_BAD_UNIT);
- 
- fujinet-network.h
- 56:uint8_t fn_error(uint8_t code);
- 
- msdos/src/fn_network/fn_error.c
- 6:uint8_t fn_error(uint8_t code)
- 
- pmd85/src/bus/fuji_get_error.c
- 29:    return fn_error(dwread(&err, 1) ? err : BUS_ERROR) != FN_ERR_OK;
- 
- pmd85/src/bus/fuji_get_response.c
- 31:    return !fn_error(dwread(buf, len) ? BUS_SUCCESS : BUS_ERROR);
- 
- pmd85/src/bus/network_get_error.c
- 37:    return fn_error(dwread(&err, 1) ? err : BUS_ERROR);
- 
- pmd85/src/bus/network_get_response.c
- 37:    return fn_error(dwread(buf, len) ? BUS_SUCCESS : BUS_ERROR);
- 
- pmd85/src/fn_network/fn_error.c
- 4:uint8_t fn_error(uint8_t code)


----

[Back to index](../index.md)
