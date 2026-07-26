# Tooling guidance

These scripts configure targets, prepare firmware, generate indexes/deltas, and
support initial provisioning. Several mutate ignored build state or artifacts.

- Read the entire script and its CI caller before changing behavior.
- Keep shell scripts non-interactive in CI paths, quote filenames/arguments,
  propagate failures, and avoid leaking Wi-Fi credentials or key material.
- `switch_to.sh` deletes `build/`, `sdkconfig`, and `sdkconfig.defaults`; do not
  run it as a read-only probe.
- `check_default_configs.sh` regenerates and rewrites canonical config files.
  Run it only for configuration/Kconfig changes, then review every diff.
- Treat `initial_flash_*`, firmware preparation, patching, and index generation
  as artifact-sensitive. Do not flash, sign, or publish as a side effect of a
  test.
- Keep generated output deterministic and compatible with CI's pinned ESP-IDF
  version.

Relevant checks include `./tools/check_default_configs.sh`,
`(cd tools/bip85_rsa_key_gen && ./test.sh)`, targeted Python invocation, and
`git diff --check`. Choose only checks matching the touched tool.

