# Component guidance

This tree mixes repository-owned components with git submodules.

External submodule boundaries:

- `esp32_bc-ur`
- `esp32_bsdiff`
- `esp32_deflate`
- `libwally-core/upstream`

Do not edit their contents or update gitlinks unless explicitly requested.
Prefer adapting the owned integration layer or CMake configuration when that is
the intended scope.

Owned generators include `assets/` and `autogenlang/`. For asset registry
updates, the sorted JSON files are source; generated `.inc` files belong in the
build directory. For localization, update the source mapping/generator and
regenerate matching C/header output rather than hand-editing generated files.

When changing a component, inspect its license, Kconfig, CMake registration, and
dedicated test directory. Build every architecture that consumes the changed
code and do not silently widen component dependencies.

