# Fujinet Lib

[![Latest release](https://img.shields.io/github/v/release/FujiNetWIFI/fujinet-lib?label=library)](https://github.com/FujiNetWIFI/fujinet-lib/releases/latest)
[![Test images](https://img.shields.io/badge/test%20images-tests--latest-blue)](https://github.com/FujiNetWIFI/fujinet-lib/releases/tag/tests-latest)

Routines for using the FujiNet Adapter sub-device.

The public headers are in `include/`, code shared by every platform in `common/`, and each
platform's bus code in `bus/<platform>/`.

## Building

`make release` builds every platform's zip into `release/`; `make <platform>/release` builds one.

```shell
$ make clean
$ make release
$ make atari/release
```

## Releasing a new version of the library

### Update the changelog

Update the changelog in the `Changelog.md` file.

Put notes in and create a dated section for the new version.

### Commit and push the changes

Once this is done, you can then cause a release to be built by tagging the release, as
described below.

### Tag the release

Pushing a `v*` tag starts the GitHub release workflow. The tag must match
`FNLIB_VERSION_FULL` in `include/fujinet-version.h`, and `Changelog.md` must have a
section for it.

Note, for the following to work, you must have a git remote named "upstream" pointing to
the GitHub repository, and have permissions to push tags.

```shell
git tag vX.Y.Z
git push upstream vX.Y.Z
```

### Version string

Release zip and library names use `FNLIB_VERSION_FULL` from `include/fujinet-version.h`.
To build a local release under another name, set `RELEASE_VERSION`:

```shell
make RELEASE_VERSION=v4.11.3 c64/release
```

## Testing

Testing applications are available for each platform. Run `make -C testing` to
build them all, or `make -C testing <platform>` to build one platform.

Every push to `main` rebuilds them and recreates the `tests-latest` prerelease,
so it stays at the top of the releases list and the current images are always at
https://github.com/FujiNetWIFI/fujinet-lib/releases/download/tests-latest/fnlib-tests-latest.zip
as `<platform>/<image>`.
