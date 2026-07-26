# Configuration guidance

Files here are canonical ESP-IDF defaults for official, production, QEMU, and
DIY targets.

- Change Kconfig symbols in `main/Kconfig.projbuild` when introducing a feature;
  update only affected defaults.
- Preserve the active Jade v2 configuration unless the task requires switching.
- Keep ESP32 and ESP32-S3 constraints distinct. Shared display/input/power code
  may require representative builds from both families.
- Production defaults must not enable debug handlers, unattended CI actions,
  development signing, verbose secret-bearing logs, or weakened security
  options.
- Do not edit ignored root `sdkconfig`, `sdkconfig.defaults`, or
  `sdkconfig.old` as the source of truth.

After config changes, run `./tools/check_default_configs.sh` in a disposable or
cleanly understood build state. It removes build/config state and rewrites
defaults. Review `git diff` and build the affected target matrix.

