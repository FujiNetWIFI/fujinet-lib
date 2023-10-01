# Changelog

## [Unreleased]

### Added

- Moved BDD src to separate project as it shared over multiple projects now
- Added apple-single testing feature
- Added Smart Port emulator for feature tests to be able to run apple2 fujinet code
- Added network_open for apple2
- Added network_close for apple2
- Added network_status for apple2
- Create set of standard error codes that device specific values get converted to

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
