---
name: jade-build-config
description: Maintain or diagnose Jade ESP-IDF builds, Kconfig/defaults, board targets, CMake/component registration, amalgamated builds, QEMU, libjade, GitLab/GitHub CI, firmware packaging, and release tooling. Use for build failures, new feature flags or boards, dependency changes, config normalization, CI matrix edits, firmware artifacts, or inspection/testing of signing and release scripts.
---

# Jade Build and Config

Preserve target intent and keep local validation aligned with CI without causing
hardware or release side effects.

## Prepare

Read the root plus the scoped guidance for `configs/`, `tools/`, `gitlab/`,
`.github/`, `release/`, and any changed component. Read
[references/build-matrix.md](references/build-matrix.md).

## Workflow

1. Classify the task as source registration, feature Kconfig, board defaults,
   toolchain/dependency, native build, QEMU, CI, packaging, or release.
2. Preserve the active Jade v2 configuration unless the task requires a target
   switch. Remember that `switch_to.sh` deletes current build/config state.
3. Put source-of-truth changes in CMake/Kconfig/canonical defaults, never ignored
   generated configuration or build output.
4. Check amalgamated and unamalgamated inclusion for new firmware sources.
5. Keep ESP32/ESP32-S3, production/development, radio/no-radio, CI,
   PSRAM/no-PSRAM, and debug boundaries explicit.
6. Mirror commands and artifact paths between local workflows and CI.
7. Treat release scripts as inspect/test only unless the user explicitly
   authorizes signing, promotion, publishing, or remote interaction.
8. Review the final config/artifact diff for unintended security-option changes.

## Validate

Run the narrow build first, then the affected matrix. Use
`check_default_configs.sh` only when configuration sources changed because it
rewrites defaults and deletes local build/config state. Never flash, sign,
promote, or publish as implicit validation.

