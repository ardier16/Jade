# Input abstraction guidance

The `.inc` implementations map wheel, rocker/navigation, select-only, single
button, and touchscreen hardware to common GUI events.

- Preserve press/release/long-press semantics, debounce behavior, focus
  direction, and cancellation behavior.
- Do not make a confirmation easier to trigger accidentally on one device.
- Check every Kconfig-selected implementation that consumes a changed shared
  event or button ID.
- Validate interactive behavior in QEMU/libjade where possible and compile
  representative ESP32 and ESP32-S3 targets for shared changes.

