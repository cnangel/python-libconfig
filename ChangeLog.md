# ChangeLog

## [0.2.1] — 2026-05-16

### Fixed
- Fix version guards for multi-version libconfig compatibility (1.1.x ~ 1.8.x)
- Fix `isString()` implementation to use `getType()` instead of non-existent API

## [0.2.0] — 2026-05-15

### Added
- Upgrade to support libconfig 1.1.x ~ 1.8.x
- Export `Setting` class with full API
- Add type-safe lookup methods (`lookupInt`, `lookupString`, `lookupFloat`, `lookupBool`, `lookupInt64`)
- Add options API (`setOption`, `getOption`, `setOptions`, `getOptions`)
- Add format control (`setDefaultFormat`, `getDefaultFormat`)
- Add `setFloatPrecision`, `setTabWidth`, `setAutoConvert`, `clear`
- Export type/format/option constants
- Add GitHub Actions CI/CD workflows

### Changed
- Use `setValue` with Python type introspection dispatching

### Fixed
- Fix `children()` shadowed declaration
- Fix `setValue` type dispatch for unsigned int
- Fix `add*` methods with empty path (use root setting)

## [0.0.4] — 2016-02-28

### Added
- Add `addList` support

## [0.0.3] — 2015-08-15

### Added
- Add `readString()` support with libconfig version checking

## [0.0.2] — 2014-06-10

### Fixed
- Fix boost_python library detection (default to libraries if not found)
- Fix Linux Mint 16 + Python 2.7 compatibility
- Fix Fedora 20 build

## [0.0.1] — 2010-04-16

### Added
- Initial release
- Basic `Config` bindings via Boost.Python
- Read/write/add/setValue/value operations
- DEB and RPM packaging