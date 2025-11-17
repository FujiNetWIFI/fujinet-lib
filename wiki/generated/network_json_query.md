# network_json_query

**Declared in:** `fujinet-network.h`

## Prototype

```c
/**
 * @brief  Perform JSON query
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @param  query pointer to string containing json path to query, e.g. "/path/field". No need to add device drive.
 * @param  s pointer to receiving string, nul terminated, if no data was retrieved, sets it to an empty string
 * @return Bytes read, or negative values represent fujinet-network error code (See FN_ERR_* values)
 * 
 * Assumes an open and parsed json.
 */
int16_t network_json_query(const char *devicespec, const char *query, char *s);
```

## Description

_No description available — please add a detailed description of what this function does._

## Parameters

| Name | Type | Description |
|---|---|---|
| `*devicespec` | `const char` | _TODO: describe parameter_ |
| `*query` | `const char` | _TODO: describe parameter_ |
| `*s` | `char` | _TODO: describe parameter_ |

## Return Value

- **Type:** `int16_t`

- **Meaning:** _TODO: describe return value and error conditions._

## Header notes

```
/**
 * @brief  Parse the currently open JSON location
 * @param  devicespec pointer to device specification, e.g. "N1:HTTPS://fujinet.online/"
 * @return fujinet-network error code (See FN_ERR_* values)
 * 
 * This will set the channel mode to JSON, which will be unset in the close.
 */
```

## Implementations (git grep results)

- Changelog.md
- 57:- [coco] fix network_json_parse and network_json_query (upstream/main) [Jan Krupa]
- 
- adam/src/fn_network/network_json_query.c
- 19:int16_t network_json_query(const char *devicespec, const char *query, char *s)
- 
- apple2/src/fn_network/network_json_query.c
- 8:int16_t network_json_query(const char *devicespec, const char *query, char *s) {
- 
- atari/src/fn_network/network_json_query.s
- 25:; int16_t network_json_query(const char *devicespec, const char *query, char *s);
- 
- coco/src/fn_network/network_json_query.c
- 8:int16_t network_json_query(const char *devicespec, const char *query, char *s)
- 
- commodore/src/fn_network/network_json_query.c
- 10:int16_t network_json_query(const char *devicespec, const char *query, char *s)
- 
- fujinet-network.h
- 167:int16_t network_json_query(const char *devicespec, const char *query, char *s);
- 
- msdos/src/fn_network/network_json_query.c
- 6:int16_t network_json_query(const char *devicespec, const char *query, char *s)
- 
- pmd85/src/fn_network/network_json_query.c
- 7:int16_t network_json_query(const char *devicespec, const char *query, char *s)


----

[Back to index](../index.md)
