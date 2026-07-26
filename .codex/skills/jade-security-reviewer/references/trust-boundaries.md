# Jade trust boundaries

## Protected assets

- Mnemonic entropy, seed, derived private keys, and passphrase state.
- PIN private key, encrypted wallet blob, replay/counter state, and pinserver
  session material.
- Signing nonces, anti-exfil commitments/entropy, hashes, and signatures.
- Liquid master blinding key, blinding factors, nonces, and commitments.
- Registered multisig/descriptor/OTP state and wallet/network restriction.
- Attestation keys and firmware update/signing trust roots.
- User intent represented by on-device confirmation.

## Untrusted inputs

- CBOR over serial, BLE, TCP/libjade, and internal continuation messages.
- Transaction, PSBT/PSET, descriptor, multisig, identity, message, and QR data.
- Camera frames and QR/BC-UR parsing.
- USB mass-storage filenames and file contents.
- Pinserver/HTTP responses, URLs, certificates, and network errors.
- NVS contents after corruption or unexpected version.
- Firmware images, deltas, indexes, signatures, and release artifacts.
- Build configuration, submodule revisions, and generated component data.

## Review invariants

### Authentication and state

- Sensitive methods require an initialized/unlocked wallet and the same message
  source that authorized it.
- Temporary and persisted wallets do not accidentally share state or policy.
- Network restriction is checked before derivation/signing/export.
- Logout, reset, failure, and source changes clear sensitive runtime state.

### Parsing and memory

- Every host length/count is bounded before allocation, copying, multiplication,
  or iteration.
- Integer conversions and amount/fee arithmetic cannot wrap or truncate.
- Ownership is unambiguous; cleanup covers parse errors, user rejection, crypto
  errors, and success.
- Secret buffers are not logged and are cleared as early as feasible.

### User verification

- The device derives and displays external outputs, amounts/assets, fees,
  network, warnings, and operation identity.
- Host labels/metadata are bounded and never substitute for verified facts.
- Cancel/back paths cause no signature/export/state change.
- Debug unattended confirmation remains impossible in production builds.

### Cryptography

- Randomness and nonce generation return values are checked.
- Nonces/commitments are not reused and anti-exfil sequencing is bound to the
  original request.
- Domain separation, hash construction, sighash selection, signature encoding,
  and key derivation match the protocol.
- Comparisons that protect secrets/authentication use appropriate constant-time
  primitives where required.

### Persistence and update

- Missing/corrupt NVS values produce safe defaults or explicit failure.
- Counters, PIN attempts, and replay data cannot be rolled back casually.
- OTA/delta images are authenticated before activation and downgrade/partition
  policy remains intact.
- Bootloader/eFuse/trust-root assumptions match production configuration.

### Build and release

- Debug handlers, logs, JTAG, CI auto-actions, and development keys cannot enter
  production artifacts unintentionally.
- Firmware signing thresholds, key order, hashes, indexes, and versions remain
  deterministic and verified.
- Signing, promotion, publishing, flashing, and production key operations are
  explicitly authorized, never incidental tests.

## Evidence expectations

Prefer a regression that demonstrates the rejected attack or preserved
invariant. libjade and QEMU provide strong functional evidence but cannot prove
physical secure storage, entropy quality, exact peripheral behavior, timing,
power analysis, fault injection resistance, or side-channel safety.

