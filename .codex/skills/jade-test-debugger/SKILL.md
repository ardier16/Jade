---
name: jade-test-debugger
description: Reproduce, isolate, and verify Jade firmware, signing, protocol, UI, memory, and integration failures using test_jade.py, test_data vectors, native libjade, sanitizers, the libjade GUI, and QEMU before physical hardware. Use when a Jade test fails, a signing fixture regresses, behavior differs by legacy/anti-exfil flow, an emulator is needed, or a safe validation plan must be chosen.
---

# Jade Test Debugger

Use the cheapest faithful environment first and escalate only when the behavior
depends on hardware.

## Prepare

Read the root, `libjade/`, `main/qemu/`, and `test_data/` guidance. Read
[references/test-matrix.md](references/test-matrix.md).

## Workflow

1. Record the exact failing command, fixture, build type, target, feature flags,
   legacy/anti-exfil mode, and first meaningful error.
2. Determine whether the failure is vector-driven, high-level API behavior,
   low-level malformed input, UI/event behavior, native runtime behavior, QEMU
   behavior, or target-only compilation/runtime behavior.
3. Reproduce with libjade and the smallest matching `--json-filter` when
   applicable. Do not assume individual `test_*` functions are pytest tests;
   `test_jade.py` is its own harness.
4. Compare the request/response boundary and fixture assumptions before editing
   code or expected data.
5. Use the libjade GUI or QEMU for user-event/display issues.
6. Use a sanitizer build for ownership, lifetime, bounds, or undefined-behavior
   suspicions.
7. Build the preserved Jade v2 target after native tests pass.
8. Add a regression case that fails for the original reason and succeeds for
   the fix.

## Report

State the root cause, environment(s) reproduced, exact tests run, why the chosen
emulator was faithful enough, and which hardware-only behavior remains
unverified. Never silently reduce `--sample-percent`, skip a failing family, or
weaken an assertion to obtain a pass.

