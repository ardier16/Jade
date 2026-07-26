---
name: jade-security-reviewer
description: Threat-model and review Jade hardware-wallet changes involving CBOR/RPC input, authentication/PIN, keychain/storage, cryptographic signing, anti-exfil, wallet/network/path policy, confirmation UI, OTA/bootloader, USB/BLE/serial/network transports, attestation, build flags, firmware signing, or release trust roots. Use for security reviews, risky implementation plans, regression analysis, and pre-merge review of sensitive firmware or client changes.
---

# Jade Security Reviewer

Review concrete attack paths and invariant violations, not generic secure-coding
style.

## Prepare

Read the root and all scoped agent files for the diff. Read
[references/trust-boundaries.md](references/trust-boundaries.md). Inspect the
callers, consumers, tests, configuration gates, and legacy behavior surrounding
each changed line.

## Review workflow

1. Identify assets: mnemonic/seed, private keys, PIN-protected blob, nonces,
   blinding data, attestation/update keys, user intent, and device availability.
2. Identify attacker-controlled boundaries: host CBOR, serial/BLE/TCP, camera/QR,
   USB media, pinserver/network, firmware image, persisted storage, and build
   artifacts.
3. Trace authentication, authorization, device state, network/path policy, and
   user confirmation before sensitive use or export.
4. Check lengths, counts, arithmetic, allocation, ownership, cleanup, error
   paths, replay/state sequencing, and request/source binding.
5. Check that displayed facts are complete, device-verified, and bound to the
   exact operation signed.
6. Check crypto API return values, randomness/nonces, domain separation,
   anti-exfil sequencing, constant-time expectations, and secret lifetime.
7. Check production/debug/Kconfig boundaries, OTA/bootloader trust, release
   signatures, and downgrade behavior.
8. Require positive and adversarial tests that demonstrate the invariant.

## Findings

Report only actionable findings. For each, give severity, exact file/line,
attacker capability, violated invariant, concrete consequence, and the smallest
credible remediation/test. Distinguish confirmed defects from open questions.
State residual hardware or side-channel risks that libjade/QEMU cannot verify.

Do not perform signing, flashing, publication, production key operations, or
submodule updates during a review.

