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

## Adding additional build flags to local release

Edit the `application.mk` and add any options you need to build a release with additional flags.
The `hd` function (for hex dumping) is guarded by the `FN_LIB_DEBUG` define.

```makefile
# CFLAGS += -DFN_LIB_DEBUG
```

This is not enabled by default, so release libraries will not contain the hd function.
If you are doing local testing, this is the place to add custom CFLAGS or ASMFLAGS for
the built library to pickup.

## Releasing a new version of the library

This can be done with the gitlab automated workflow.
To trigger it, tag the release with a version and push the tag to the remote.

Note, for the following to work, you must have a git remote named "upstream" pointing to
the gitlab repository, and have permissions to push tags.

```shell
git tag vX.Y.Z
git push upstream vX.Y.Z
```

## Testing

Testing is WIP.
See [Testing README](testing/unit/README.md)
