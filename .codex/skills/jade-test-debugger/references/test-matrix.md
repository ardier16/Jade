# Test and emulator matrix

| Environment | Best for | Does not prove |
|---|---|---|
| Focused libjade vector | Transaction/protocol regression and fast iteration | Hardware timing, secure storage, exact UI |
| Full libjade suite | Shared firmware/client behavior and memory-stat regressions | Physical peripherals and side channels |
| Sanitized libjade | Bounds, ownership, leaks/UB indicators | Production toolchain behavior |
| libjade GUI | Activity flow, input events, camera/NVS development | Exact device rendering/security |
| QEMU + TCP | ESP32 firmware integration and optional web display | Jade v2/S3 hardware fidelity |
| Jade v2 `idf.py all` | ESP32-S3 target compilation/link/size | Runtime behavior |
| Other target builds | Shared board/config compatibility | Runtime behavior |
| Physical device | Hardware-only runtime and peripherals | Allowed only by explicit request |

## Native setup

The current Python loader explicitly opens `libjade.so`, and the helper scripts
use Linux `LD_LIBRARY_PATH`/sanitizer conventions. Run this path on Linux or in
the repository's builder container. On macOS, prefer Docker-backed QEMU unless
you intentionally enter a Linux environment.

Build:

```sh
./libjade/make_libjade.sh Debug
```

On Linux, expose the shared library:

```sh
export LD_LIBRARY_PATH="$PWD/build_linux/libjade${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
```

Then run either a fixture family or the full harness:

```sh
python test_jade.py --log CRITICAL --libjade --json-filter 'pattern'
python test_jade.py --log CRITICAL --libjade
python test_jade.py --log CRITICAL --libjade --nolegacyflow
```

`--json-filter` is a glob under `test_data/`. It intentionally selects
vector-driven high-level cases and skips unrelated tests. Use a full run for
RPC framing, negative input, authentication, transport, or non-vector behavior.

## Fixture routing

- `tx_*`, `txn_*`: Bitcoin transaction signing.
- `liquid_txn_*`, `tx_liquid_*`: Liquid signing and commitments.
- `psbt_*`: Bitcoin PSBT signing.
- `pset_*`: Liquid PSET signing.
- `descriptor_*`: descriptor registration and signing policy.
- `multisig_*`: registration, parsing, address, and signature behavior.
- `msg*`: message and message-file signing.
- `identity_*`: identity key/signature flows.
- `qr_*`: QR parsing/camera data.

## QEMU

`./main/qemu/run_emulator.sh` performs a Docker-backed copied-tree build and
exposes Jade TCP on `30121` and the web display on `30122`. A direct
`switch_to.sh qemu` replaces active local build/config state.

Use `test_jade.py --qemu --serialport <tcp-or-pty>` according to the launched
connection. Keep `--qemu` so the harness skips hardware-only cases.

## Sanitizers

CI builds libjade with `Sanitize`, sets ASAN/UBSAN runtime variables, and runs
standard, non-legacy, and daemon/serial-style suites. Reproduce the smallest
failing form locally first. Platform sanitizer library paths differ; use the
build's printed guidance or `gitlab/test_libjade.yml`.

## Triage rules

- Preserve the first assertion/error and device log around it.
- Confirm the failure with one fixture before widening.
- Compare legacy and `--nolegacyflow` only when the code path differs.
- Do not update expected signatures or serialized data until the contract and
  cryptographic inputs are verified.
- Distinguish emulation gaps from product defects and keep a regression test for
  confirmed defects.
