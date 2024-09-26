# Fujinet Lib

Routines for using the FujiNet Adapter sub-device.

The top level folder contains the .h API, one subfolder for each platform.

Each fujinet device is then a subdirectory of the src directory, e.g. clock, fuji, network.

## Building

If required, specify `TARGETS` list.

```shell
$ make clean
$ make release
```

## Testing

Testing is done with BDD features. See [Testing README](testing/bdd-testing/README.md)
