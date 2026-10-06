# Fujinet Lib

Routines for using the FujiNet Adapter sub-device.

The top level folder contains the .h API, one subfolder for each platform.

Each fujinet device is then a subdirectory of the src directory, e.g. clock, fuji, network.

## Building

If required, specify `TARGETS` list.

```shell
$ make clean
$ make release
$ make TARGETS=atari release
$ make TARGETS="c64 atari" release
```

## Releasing a new version of the library

### Update the changelog

Update the changelog in the `CHANGELOG.md` file.

Put notes in and create a dated section for the new version.

### Commit and push the changes

Once this is done, you can then cause a release to be built by tagging the release, as
described below.

### Tag the release

Release artifact names are derived from the git tag when the build runs on a clean
checkout at that tag (for example tag `v4.9.0` produces version `4.9.0` in zip and
library filenames). Builds from other commits use the short git hash instead.

The new release can be done with the gitlab automated workflow.
To trigger it, tag the release with a version and push the tag to the remote.

Note, for the following to work, you must have a git remote named "upstream" pointing to
the gitlab repository, and have permissions to push tags.

```shell
git tag vX.Y.Z
git push upstream vX.Y.Z
```

### Version string

This is advanced usage if you're producing local versions of fujinet-lib library and
require specific version names.

The `release` build will use either the latest tag name (if this is also the current commit)
or the short commit hash string if locally there is a commit without the latest tag on it
(adding "-dirty" if you have uncommit changes).

```shell
❯ make TARGETS="c64 msdos" release
# ...

❯ ll dist
Permissions Size User  Date Modified Name
.rw-r--r--   59k markf 11 Jun 16:29   fujinet-lib-c64-45c687eb-dirty.zip
.rw-r--r--   29k markf 11 Jun 16:29   fujinet-lib-msdos-45c687eb-dirty.zip
```

To build with a specific version string in the files instead of the hash, specify the VERSION_STRING
env directly **using just the number without 'v'** (along with any other env values, like the TARGETS if you wish to only build limited set):
```shell
❯ make VERSION_STRING=4.11.3 TARGETS="c64 msdos" release
# ...

❯ ll dist
Permissions Size User  Date Modified Name
.rw-r--r--   59k markf 11 Jun 16:28   fujinet-lib-c64-4.11.3.zip
.rw-r--r--   29k markf 11 Jun 16:28   fujinet-lib-msdos-4.11.3.zip
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

## Testing

Testing applications are available for each platform. Run `make -C testing` to
build them all, or `make -C testing <platform>` to build one platform.
