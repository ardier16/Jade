# Firmware guidance

This directory is the security boundary of the device firmware.

## Architecture

- `process.c` owns inbound/outbound message queues, CBOR replies, process
  lifecycle, and deferred cleanup. Individual RPC handlers are in `process/`.
- `wallet.c`, `signer.c`, `keychain.c`, `storage.c`, and `identity.c` handle
  cryptographic and persisted state. Preserve their separation of concerns.
- `ui/` builds activities; process/dashboard code generally drives event loops
  and applies state changes.
- `input/`, `power/`, `display*`, and `camera*` select hardware-specific
  implementations through Kconfig.
- `amalgamated.c` includes firmware sources for the default build. New source
  files must work in both amalgamated and unamalgamated builds and be registered
  by `main/CMakeLists.txt` when necessary.

## Firmware invariants

- Validate all host-controlled lengths, types, counts, networks, paths, and
  state transitions before using them.
- Keep secret lifetime short. Use existing secure allocation/cleanup patterns,
  register process cleanup immediately after allocation, and clear transient
  key material on every exit.
- Check return values from storage, crypto, transport, and ESP-IDF APIs. Follow
  existing `JADE_ASSERT`, `JADE_WALLY_VERIFY`, and recoverable error patterns.
- Preserve message-source ownership and do not reply on the wrong serial, BLE,
  internal, or QEMU channel.
- Do not add secret-bearing values to logs, assertions, screenshots, or CBOR
  error data.
- Consider no-PSRAM and radio-disabled builds when changing memory use or
  component dependencies.

## Validation

Run `./format.sh` only when prepared to review all formatting changes it makes.
Prefer an unamalgamated build when diagnosing missing declarations or source
registration. Use libjade for logic, QEMU/libjade GUI for interaction, and the
preserved Jade v2 build for target compilation. Physical flashing requires an
explicit request.

