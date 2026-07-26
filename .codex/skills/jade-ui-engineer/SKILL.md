---
name: jade-ui-engineer
description: Design, implement, or debug Jade firmware screens, menus, dialogs, carousels, signing confirmations, keyboards, QR/camera flows, and input interactions across Jade and DIY display variants. Use for UI/UX work in main/ui, dashboard event loops, button events, layout, focus, cancellation, long text, wheel/navigation/select/touch behavior, or visual checks in libjade/QEMU.
---

# Jade UI Engineer

Build UI flows that preserve both usability and the security meaning of device
confirmation.

## Prepare

Read the root, `main/`, `main/ui/`, and relevant `main/input/` guidance. Read
[references/ui-map.md](references/ui-map.md). For signing pages, also use
`jade-signing-engineer`.

## Workflow

1. Find the nearest activity with the same interaction and screen density.
2. Define what information is device-derived, host-provided, security-critical,
   optional, or too long for one page.
3. Keep screen construction in `main/ui/` and stateful event handling in the
   owning process/dashboard flow, matching established patterns.
4. Reuse title bars, button layouts, fonts, colors, menu/carousel helpers, and
   click/back event helpers.
5. Specify focus order, default focus, left/right movement, click, back/cancel,
   long press, and touch behavior.
6. Preserve activity/node lifetime and release prior screens according to the
   navigation loop's ownership model.
7. Check small and large displays, empty/maximal values, long labels,
   untrusted strings, and multiline content.
8. Keep unattended CI behavior behind the existing debug CI configuration.

## Validate

- Build and interact with `./libjade/run_libjade_gui.sh Debug` for rapid checks.
- Use `./main/qemu/run_emulator.sh` when the web display or QEMU input path adds
  coverage.
- Compile Jade v2/ESP32-S3. Build representative ESP32 targets when shared
  layout/input/power abstractions change.
- Re-run the owning protocol/feature tests; screenshots alone are insufficient.

Do not flash a physical device unless explicitly requested.

