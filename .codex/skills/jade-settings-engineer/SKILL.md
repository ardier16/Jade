---
name: jade-settings-engineer
description: Implement or modify end-to-end Jade firmware settings, including menu entries, button events, persistence/defaults, runtime consumers, and locked/unlocked visibility. Use for requests such as adding a settings option, changing an existing preference, persisting device behavior, or debugging settings state across main/ui/dashboard.c, main/process/dashboard.c, storage, keychain, and related firmware code.
---

# Jade Settings Engineer

Implement settings as complete state paths rather than isolated menu rows.

## Prepare

Read the root, `main/`, and `main/ui/` agent guidance. Read
[references/settings-flow.md](references/settings-flow.md) before editing.
Also use `jade-ui-engineer` when the interaction is more than an existing
toggle/carousel pattern and `jade-security-reviewer` when the setting changes
authentication, wallet, network, signing, transport, or secret handling.

## Workflow

1. Define the setting's type, safe default, persistence lifetime, visibility,
   and behavior for missing/invalid stored data.
2. Trace the closest existing setting through event ID, menu builder, event
   loop, persistence API, and runtime consumer.
3. Decide whether the value belongs in NVS storage, keychain flags, runtime
   state, Kconfig, or a combination. Avoid parallel sources of truth.
4. Add the event ID and menu item without relying on fragile positional indexes.
5. Build the interaction from existing activities and commit state only on the
   established confirmation action. Preserve cancel/back semantics.
6. Apply changes to loaded in-memory state when needed; persistence alone must
   not leave the current session stale.
7. Cover uninitialized, locked, unlocked, temporary signer, QR/internal-source,
   and persisted-wallet states relevant to the setting.
8. Verify the consumer uses the safe default before any value has been stored.

## Validate

- Format the touched C files and inspect the exact diff.
- Build libjade and exercise the state transition through the libjade GUI when
  the setting is interactive.
- Use QEMU web display for layout/input checks when helpful.
- Build the preserved Jade v2 configuration with `idf.py all`.
- Add automated coverage when the setting affects an RPC-observable behavior;
  otherwise document the manual emulator checks performed.

Never flash physical hardware as part of this workflow unless explicitly
requested.

