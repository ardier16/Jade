# Build, configuration, and CI map

## Official targets

| Target | Chip | Development defaults | Production defaults |
|---|---|---|---|
| `jade` | ESP32 | `configs/sdkconfig_dev_jade.defaults` | `configs/production/sdkconfig_jade_prod.defaults` |
| `jade_v1_1` | ESP32 | `configs/sdkconfig_dev_jade_v1_1.defaults` | `configs/production/sdkconfig_jade_v1_1_prod.defaults` |
| `jade_v2` | ESP32-S3 | `configs/sdkconfig_dev_jade_v2.defaults` | `configs/production/sdkconfig_jade_v2_prod.defaults` |
| `jade_v2c` | ESP32-S3 | `configs/sdkconfig_dev_jade_v2c.defaults` | `configs/production/sdkconfig_jade_v2c_prod.defaults` |
| `qemu` | ESP32 | `configs/sdkconfig_qemu.defaults` | development only |

The current workspace default is Jade v2/ESP32-S3. Preserve it unless a target
switch is part of the task.

## Source-of-truth map

- Feature symbols and dependencies: `main/Kconfig.projbuild`.
- Firmware source/component registration: `main/CMakeLists.txt`.
- Project selection: root `CMakeLists.txt`.
- Canonical defaults: `configs/` and `configs/production/`.
- Dependency locks: `dependencies.lock.esp32*`.
- Active generated state: ignored `sdkconfig*`, `build/`, and
  `managed_components/`; never edit as source.
- Native build: `libjade/CMakeLists.txt` and `libjade/make_libjade.sh`.
- Target switch/generation: `tools/switch_to.sh`,
  `tools/check_default_configs.sh`.

## Important side effects

`tools/switch_to.sh` cleans IDF output and removes/replaces active
`sdkconfig`/`sdkconfig.defaults`. `tools/check_default_configs.sh` removes build
and managed-component state, regenerates defaults, sorts them, and rewrites
canonical files. Run either only when those mutations are intended.

## CI matrix

- GitHub Actions: public development builds for Jade 1.1, v2, and v2c with the
  pinned ESP-IDF version.
- GitLab production/dev jobs: official target and radio/no-radio firmware.
- GitLab test builds: unattended/debug target variants and QEMU variants.
- `test_libjade.yml`: native, sanitizer, daemon, non-legacy, and coverage paths.
- `test.yml`: formatting, canonical config diff, and BIP85 RSA tool tests.
- `diy_fw.yml`: representative community-board compilation.
- `flash.yml`: runner-scoped physical hardware; never invoke implicitly.
- `release.yml`: manual artifact staging; signing/promotion remains explicitly
  authorized only.

## Matrix selection

- Pure firmware logic: libjade, full native suite, Jade v2 build.
- Shared hardware abstraction: Jade v2 plus representative ESP32 and affected
  DIY/official targets.
- Kconfig/default: regenerate configs, build every changed default family.
- New source: amalgamated and `--unamalgamated` builds.
- Memory/component dependency: PSRAM and no-PSRAM, radio and no-radio as
  applicable.
- Public Python packaging: `python -m build`.
- Docs: `make -C docs html`.
- Release tooling: fixture/dry-run validation only unless authorized.

## Production boundaries

Production defaults must not enable `CONFIG_DEBUG_MODE`,
`CONFIG_DEBUG_UNATTENDED_CI`, development signing, or verbose diagnostic
features that change the trust model. Inspect config diffs for secure boot,
rollback, logging, radio, JTAG, and key/trust-root changes.

