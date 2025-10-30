# running unit tests

Unit tests are a work in progress and use the soft65c02 project.

The repo for installing the unit test framework is https://github.com/chanmix51/soft65c02
You will need rust, and to follow the installation instructions for soft65c02.
When installed, you will have 2 executables on the path; soft65c02_unit, soft65c02_tester.

## running individual tests

```bash
export WS_ROOT=`realpath /path/to/fujinet-lib`
export UNIT_TEST_DIR=`realpath testing/unit`
soft65c02_unit -v -b /tmp/fujinet-lib-tests -i path/to/test_time.yaml
```

## running all unit tests

```bash
make unit-test
```

## creating unit tests

Unit tests go in `testing/unit/<platform>/<device>/<function>/`

