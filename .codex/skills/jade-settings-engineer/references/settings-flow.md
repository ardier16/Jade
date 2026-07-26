# Settings flow map

Use this map to trace a setting from presentation to effect.

| Layer | Primary locations | Responsibility |
|---|---|---|
| Event identity | `main/button_events.h` | Stable `BTN_SETTINGS_*` event ID |
| Menu/activity | `main/ui/dashboard.c` | Rows, labels, status content, button layout |
| Navigation/state | `main/process/dashboard.c` | Visibility, event loop, confirmation, runtime updates |
| Persistence | `main/storage.h`, `main/storage.c` | NVS key, safe default, typed getter/setter |
| Wallet runtime | `main/keychain.h`, `main/keychain.c` | Loaded wallet flags/restrictions and persistence bridge |
| Consumer | Feature-specific firmware | Apply the value when the behavior occurs |
| Input | `main/input/*.inc`, `main/gui.c` | Common click/left/right/back semantics |

## Existing patterns

- Display brightness: storage plus display/power runtime effect.
- Idle timeout: storage plus `idletimer.c`.
- BLE and QR preferences: storage flags plus transport/UI behavior.
- Passphrase policy: keychain flags and wallet settings screens.
- Network restriction: storage and in-memory keychain state; visibility depends
  on whether a persisted non-temporary wallet exists.
- PIN entry initial state: storage preference consumed by authentication digit
  entry.

## Design questions

Answer these before editing:

- What is the safest behavior when no NVS entry exists?
- Is the value device-wide, wallet-specific, session-only, or build-time?
- Must a locked user, unlocked user, temporary signer, or uninitialized device
  be able to see/change it?
- Does changing it take effect immediately or after reboot/unlock?
- Can the setting weaken confirmation, authentication, network isolation,
  privacy, radio exposure, or secret handling?
- Is a migration needed for an old stored representation?
- What happens when a storage write fails?

## Implementation checklist

- Add a descriptive event enum; avoid reusing an unrelated event.
- Keep NVS key names within 15 characters (the storage name limit excludes the
  terminating NUL) and validate new persisted representations.
- Keep menu array indexing correct when rows are conditionally replaced or
  populated.
- Reset output node pointers before rebuilding a freed menu activity.
- Initialize the interaction from the current value.
- Change temporary UI state on left/right; persist only on confirmation.
- Preserve back/cancel without committing.
- Check the storage setter result where the surrounding flow can recover or
  report failure.
- Make getter behavior deterministic for missing, short, or invalid data.
- Update loaded runtime state if the consumer caches the value.
- Avoid making a persisted preference part of temporary signer state unless
  intentionally designed.

## Validation scenarios

- Fresh/erased storage uses the documented default.
- The value survives logout/restart when persistence is intended.
- The current session observes the change immediately when intended.
- Locked, unlocked, uninitialized, and temporary-wallet menus show only allowed
  rows.
- Left/right/click/back work on shared input abstractions.
- Long labels and values fit small and Jade v2 displays.
- Storage failure does not silently claim success or corrupt adjacent state.
