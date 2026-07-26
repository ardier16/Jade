# Firmware UI guidance

Screens here are part of the wallet's security model, not decoration.

- Keep activity construction separate from the process that owns state and
  waits for events, following nearby screens.
- Use existing GUI nodes, fonts, colors, title bars, button layouts, and event
  helpers. Do not introduce device-specific coordinates when layout helpers can
  express the design.
- Preserve cancel/back behavior, focus order, default action, and cleanup of
  every activity and node.
- Do not truncate or obscure addresses, amounts, assets, fees, networks,
  derivation/account context, warnings, or other facts a user relies on.
- Treat host labels and metadata as untrusted. Bound their length and visually
  distinguish verified from unverified data when the existing flow does so.
- Check small displays, Jade v2's 320x170 display, long translated/dynamic text,
  and wheel/navigation/select/touch input semantics.
- CI auto-selection is test behavior only; do not make production confirmation
  unattended.

For new signing pages, read `.codex/skills/jade-ui-engineer/SKILL.md` and
`.codex/skills/jade-signing-engineer/SKILL.md`. Exercise the flow with the
libjade GUI or QEMU web display and still compile the active Jade v2 target.

