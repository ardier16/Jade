# Jade Agent Guide

This is the canonical repository-wide guidance for coding agents. More specific
`AGENTS.md` files override or extend it within their directory. `CLAUDE.md`
files import the same guidance for Claude Code; do not duplicate rules there.

## Repository map

- `main/`: ESP-IDF firmware. RPC dispatch and state live in `main/process.c`
  and `main/process/`; screen builders live in `main/ui/`; wallet, keychain,
  storage, transport, display, camera, power, and input code live beside them.
- `jadepy/`: public Python client and serial, BLE, TCP, and libjade transports.
- `test_jade.py` and `test_data/`: integration harness and protocol/signing
  vectors.
- `libjade/`: native development-only implementation of the firmware runtime.
- `main/qemu/`: ESP32 QEMU runtime and optional web display.
- `configs/`: canonical board and production defaults.
- `tools/`: configuration, firmware packaging, delta, and indexing utilities.
- `components/`: owned components plus external git submodules.
- `bootloader_components/`: bootloader-specific code.
- `release/` and `gitlab/`: signing/release tooling and CI definitions.
- `docs/`, `diy/`, `hardware/`, and `logo/`: public docs and hardware/artwork
  assets.
- `pinserver/`: external `blind_pin_server` git submodule.

## Start every task

1. Read the nearest `AGENTS.md` files for every path in scope.
2. Inspect `git status --short --branch`; preserve unrelated and untracked
   user work.
3. Identify whether the change crosses firmware, client API, tests, test
   vectors, build configuration, or documentation boundaries.
4. For security-sensitive behavior, state the trust boundary and failure mode
   before editing.

Use `rg`/`rg --files` for discovery. Follow existing C and Python patterns
instead of introducing a parallel abstraction for a one-off change.

## Safety and authorization

- Treat this as hardware-wallet code. Fail closed on malformed input,
  authentication failures, network mismatches, invalid derivation paths,
  signing-policy violations, and storage errors.
- Never log or expose mnemonic material, seeds, private keys, PINs, decrypted
  blobs, signing nonces, blinding keys, or other secret buffers.
- Do not weaken user-confirmation screens or convert a user-mediated action
  into an unattended default.
- Do not flash or erase a physical device unless the user explicitly requests
  it for the current task. Never assume an attached unit is development
  hardware.
- Do not sign firmware, promote a release, publish artifacts, upload firmware,
  or perform production key operations without an explicit request and final
  confirmation of the exact operation.
- Do not modify submodule contents or gitlink revisions unless explicitly
  requested. The submodules are listed in `.gitmodules`.
- Test/dev keys and deterministic test mnemonics in the repository are fixtures;
  do not represent them as production secrets or copy them into new contexts.
- Do not edit generated build output under `build/`, `build_linux/`,
  `managed_components/`, `docs/_build/`, or ignored `sdkconfig*` files as the
  source of a change.

## Default target and configuration

Preserve the active configuration. This workspace currently targets Jade v2 on
ESP32-S3. Validate that target unless the task names another device or changes
shared display, input, power, transport, or board abstraction code.

`tools/switch_to.sh` removes `build/`, `sdkconfig`, and `sdkconfig.defaults`.
Do not run it merely to inspect a configuration. If a clean target switch is
required, explain that it replaces the active build configuration before
running it.

On this macOS workspace, initialize ESP-IDF builds with:

```sh
. ~/esp/esp-idf/export.sh
export PATH="/opt/homebrew/opt/gnu-sed/libexec/gnubin:/usr/local/opt/gnu-sed/libexec/gnubin:$PATH"
unset CMAKE_TOOLCHAIN_FILE
unset IDF_TARGET
```

Do not bake this user-specific path into product scripts.

## Validation ladder

Choose the smallest meaningful checks first, then widen them according to risk.
Report commands that could not run and why.

1. Format or syntax-check the touched language and inspect the diff.
2. Build native `libjade` for firmware logic that does not require exact
   hardware behavior, using its supported Linux environment (locally or in the
   repository's builder container):

   ```sh
   ./libjade/make_libjade.sh Debug
   ```

3. Run focused vectors when possible, then the full native harness when the
   change is broad. The library name and `LD_LIBRARY_PATH` command below are
   Linux-specific:

   ```sh
   export LD_LIBRARY_PATH="$PWD/build_linux/libjade${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
   python test_jade.py --log CRITICAL --libjade --json-filter 'relevant_glob'
   python test_jade.py --log CRITICAL --libjade
   ```

   `--json-filter` runs matching files under `test_data/`; omit it when the
   behavior is not vector-driven.
4. Use QEMU or the libjade GUI for interaction/display work. On macOS, prefer
   the Docker-backed QEMU path or run libjade in the Linux builder rather than
   assuming the `.so` runtime is native. QEMU is an ESP32 development target,
   so it complements rather than replaces a Jade v2 build:

   ```sh
   ./main/qemu/run_emulator.sh
   ./libjade/run_libjade_gui.sh Debug
   ```

5. Build the preserved Jade v2/ESP32-S3 firmware configuration:

   ```sh
   idf.py all
   ```

6. For shared board abstractions, build the affected additional targets or
   explain which matrix entries remain for CI.

Do not append `idf.py flash` to this ladder. Physical flashing is opt-in.

## Change-specific expectations

- Settings: trace button/event ID, menu construction, handler/event loop,
  storage default/persistence, runtime consumer, and locked/unlocked/temporary
  wallet visibility.
- RPC/API: update firmware parsing and replies, `jadepy` high-level API,
  negative cases, compatibility behavior, and documentation where public.
- Signing: cover legacy and anti-exfil flows where applicable, Bitcoin and
  Liquid distinctions, user-visible output/fee/change data, cancellation, and
  malformed host input.
- UI: exercise wheel, rocker/navigation buttons, select-only, and touchscreen
  semantics when shared code changes; inspect both small and 320x170 layouts.
- Configuration: keep canonical changes in `configs/` or Kconfig, regenerate
  deterministically, and review the produced diff.
- Generated assets: change the documented source and run its generator; never
  hand-edit generated output.

## Style and completion

- C/C++ formatting is governed by `.clang-format`; `format.sh` uses
  `clang-format-19` and may rewrite all supported firmware sources.
- Python follows the existing 100-column `pycodestyle` convention.
- Keep comments focused on invariants, ownership, security assumptions, and
  non-obvious device constraints.
- Avoid unrelated cleanup in security-sensitive or protocol diffs.
- Finish with a summary of behavior changed, validation performed, residual
  hardware/matrix coverage, and any explicit user action still required.
