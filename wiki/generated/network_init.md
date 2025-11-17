# network_init

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Initialise network device
 * Allows initialisation of network to perform any platform dependent checks, and allow applications to
 * exit early if there is a network issue.
 * @return fujinet-network status/error code (See FN_ERR_* values) and set device specific error if there is any
 */
uint8_t network_init(void);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

_This function takes no parameters._

## Return Value

- **Type:** `uint8_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * Convert device specific error in code to FujiNet Network library error, agnostic of device.
 * Library code calls this when it encounters an error to return value applications should use.
 */
```

## Implementations (git grep results)

- adam/src/fn_network/network_dcbs.c
- 13: * @brief pointers to network DCBs, set by network_init()
- 
- common/src/fn_network/network_init.c
- 14:uint8_t network_init(void)
- 
- fujinet-network.h
- 74:uint8_t network_init(void);


----

[Back to index](../index.md)
