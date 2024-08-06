# Fujinet Network Lib

Routines for using the FujiNet Network Adapter sub-device.

The top level folder contains the .h API, one subfolder for each platform.

## Building

If required, specify `TARGETS` list. The default is to clean, build and create release for all known platforms.

```shell
$ make
```

## Release

Use the make target `dist` which will zip the library, headers, and Changelog into version specific file at `dist/fujinet-network_VERSION.zip`.

### Disk Images

## Testing

Testing is done with BDD features. See [Testing README](testing/bdd-testing/README.md)

## TODO

### global

- [x] Makefile
- [x] .h overall API

### define API

|System | network_open() | network_close() | network_read() | network_write() | network_status() | network_ioctl() | network_json_parse() | network_json_query() | network_init() |
|-------|----------------|-----------------|----------------|-----------------|------------------|-----------------|----------------------|----------------------|----------------|
| Atari | `[x]`          |  `[x]`          | `[x]`          | `[x]`           | `[x]`            | `[x]`           | `[x]`                | `[x]`                | `[x]`          |
| Apple2| `[x]`          |  `[x]`          | `[x]`          | `[x]`           | `[x]`            | `[x]`           | `[x]`                | `[x]`                | `[x]`          |
| ADAM  | `[ ]`          |  `[ ]`          | `[ ]`          | `[ ]`           | `[ ]`            | `[ ]`           | `[ ]`                | `[ ]`                | `[ ]`          |
| CBM   | `[x]`          |  `[x]`          | `[x]`          | `[x]`           | `[x]`            | `[ ]`           | `[x]`                | `[x]`                | `[x]`          |
| Coco  | `[ ]`          |  `[ ]`          | `[ ]`          | `[ ]`           | `[ ]`            | `[ ]`           | `[ ]`                | `[ ]`                | `[ ]`          |
| rc2014| `[ ]`          |  `[ ]`          | `[ ]`          | `[ ]`           | `[ ]`            | `[ ]`           | `[ ]`                | `[ ]`                | `[ ]`          |

Listed:

- [ ] network_open()
- [ ] network_close()
- [ ] network_read()
- [ ] network_write()
- [ ] network_status()
- [ ] network_ioctl()
- [ ] network_json_parse()
- [ ] network_json_query()
- [ ] network_init()

### target: atari

- [x] network_open()
- [x] network_close()
- [x] network_read()
- [x] network_write()
- [x] network_status()
- [x] network_ioctl()
- [x] network_json_parse()
- [x] network_json_query()
- [x] network_init()

### target: apple2

- [x] network_open()
- [x] network_close()
- [x] network_read()
- [x] network_write()
- [x] network_status()
- [x] network_ioctl()
- [x] network_json_parse()
- [x] network_json_query()
- [x] network_init()

### target: adam

- [ ] network_open()
- [ ] network_close()
- [ ] network_read()
- [ ] network_write()
- [ ] network_status()
- [ ] network_ioctl()
- [ ] network_json_parse()
- [ ] network_json_query()

### target: commodore

- [x] network_open()
- [x] network_close()
- [x] network_read()
- [x] network_write()
- [x] network_status()
- [ ] network_ioctl()
- [x] network_json_parse()
- [x] network_json_query()

### target: coco

- [ ] network_open()
- [ ] network_close()
- [ ] network_read()
- [ ] network_write()
- [ ] network_status()
- [ ] network_ioctl()
- [ ] network_json_parse()
- [ ] network_json_query()

### target: rc2014

- [ ] network_open()
- [ ] network_close()
- [ ] network_read()
- [ ] network_write()
- [ ] network_status()
- [ ] network_ioctl()
- [ ] network_json_parse()
- [ ] network_json_query()
