# Changelog

## [Unreleased]

- Start adding fn_io routines to Apple2
  - fn_io_get_adapter_config
  - fn_io_get_adapter_config_extended
- Add find methods for clock, cpm, modem, printer

## [2.2.1] - 2024-02-21

- Renamed libs and archives to reflect name change from "fujinet-network" to "fujinet-lib"
- Libs are now "fujinet-{target}-{version}.lib
- Zips is now fujinet-lib-{target}-{version}.zip
- Header and include files are still fujinet-io.{h,inc} and fujinet-network.{h,inc} to represent their sub-function within fujinet-lib

## [2.2.0] - 2024-02-20

- Change signature of fn_io_mount_disk_image, and fn_io_mount_host_slot to return an error code, so all fn_io_mount* functions are consistent (fixes #2).
- [atari] fn_io_mount_disk_image returns error code from BUS
- [atari] fn_io_mount_host_slot returns error code from BUS
- [atari] added test cases for BUS errors for fn_io_mount_disk_image, fn_io_mount_host_slot, fn_io_mount_all

## [2.1.5] - 2024-01-07

### Fixed

- [apple2] sp_init multiple SP device detection (thanks @ShunKita)

## [2.1.4] - 2024-01-06

### Fixed

- [apple2] network_json_query removes any trailing CR/LF/0x9b in results as last character

### Changed

- [apple2] reduced payload memory from 1024 to 512.

## [2.1.3] - 2024-01-03

### Changed

- [apple2] sp_init looks from slot 7 down to 1 instead of up from 1 to 7
- [apple2] sp_init now additionally looks for an SP card WITH the network adapter on it, which should skip other installed SP devices
- [apple2] sp_init only runs once and stores network id, close no longer resets it.
- lots of tests fixed (cycle count errors mostly)
- add network_init to detect network errors early. Implemented on APPLE2, nop on atari.
  Note: network_open still checks if appropriate init has happened on apple, but network_init will do same thing first, and then open will use the results from init.

## [2.1.2] - 2024-01-03

### Changed

- network_read handles chunked and delayed loading, and is common cross platform C file
- refactored subdirectories of apple code in preparation for fn_io for apple2

## [2.1.1] - 2023-12-07

### Changed

- Set `_sp_network` after calling `sp_find_network` in apple2 so ioctl can work after finding a network without calling open

### Added

- Tests for network_http_* functions

## [2.1.0] - 2023-11-15

### Added

- Added network_http_add_header
- Added network_http_start_add_headers
- Added network_http_end_add_headers
- Added network_http_post
- Added network_http_put
- Added network_http_delete

- Combined fn_io.lib with fn_network.lib for single library

### Changed

- apple2 - Check if there are any devices before issuing DIB command in sp_find_device

## [2.0.0] - 2023-09-24

### Added

- Moved BDD src to separate project as it shared over multiple projects
- Added apple-single testing feature
- Added Smart Port emulator for feature tests to be able to run apple2 fujinet code
- Added network_open for apple2
- Added network_close for apple2
- Added network_status for apple2
- Added network_read for apple2
- Added network_write for apple2
- Added network_ioctl for apple2
- Create set of standard error codes that device specific values get converted to for both apple2 and atari
- Added network_json_query for atari and apple2
- Added network_json_parse for atari and apple2

Test applications that run on hardware can be found at <https://github.com/markjfisher/fujinet-network-lib-tests>
Unit tests are still run from testing/bdd-testing directory.

### Breaking

- network_status: add bw, c, err parameters
- network_open signature change: add mode

## [1.0.0] - 2023-09-24

### Added

- This changelog to instil a process of clean version documentation for the library.
- Common Makefile
- Initial implementations of atari functions:
  - network_close, network_open, network_read, network_write, network_status, network_ioctl
- cross platform BDD testing framework, atari implementation done, c64 skeleton created

## Notes

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

Use Added, Removed, Changed in triple headings.

Keep entries to lists and simple statements.
