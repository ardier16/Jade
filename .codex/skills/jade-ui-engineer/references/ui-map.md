# UI architecture map

## Layers

| Layer | Locations | Role |
|---|---|---|
| GUI primitives/tree | `main/gui.c`, `main/ui.h` | Nodes, activities, layout, paint, common widgets |
| Screen builders | `main/ui/*.c` | Construct feature-specific activity trees |
| Stateful flow | `main/process/dashboard.c`, handlers | Set current activity, wait for events, mutate state |
| Event IDs | `main/button_events.h`, GUI event definitions | Stable semantic actions |
| Hardware input | `main/input/*.inc` | Wheel, nav, select, single button, touch mapping |
| Display hardware | `main/display*.c`, Kconfig | Size, offset, framebuffer, driver behavior |
| Native/QEMU | `libjade/gui.py`, `main/qemu/` | Development interaction and visual inspection |

## Reuse targets

- Menus: `make_menu_activity`, `btn_data_t`, title/header buttons.
- Choice/toggle: carousel activities and highlighted text updates.
- Confirmation: yes/no, continue/back, skip/yes helpers.
- Text entry: keyboard and digit-entry activity helpers.
- Progress: progress bar helpers.
- Signing: `main/ui/sign_tx.c` output and final-fee activity chains.
- Address verification: `main/ui/confirm_address.c`.

## Interaction checklist

- Name the semantic event, not the physical control.
- Define initial focus and the action triggered by the common click event.
- Preserve a reachable cancel/back path before irreversible work.
- Avoid accidental confirmation from a long press or repeated/debounced event.
- Rebuild or update status nodes only while their owning activity is alive.
- Reset caller-owned node pointers when navigation frees and recreates screens.
- Keep process state outside reusable screen builders.

## Content checklist

- Mark which values are device-derived and which are host-supplied.
- Bound untrusted text and handle empty, maximal, and invalid values.
- Show complete addresses/identifiers using established paging/scrolling rather
  than unsafe truncation.
- Apply asset precision carefully; asset generator notes currently require
  single-digit precision.
- Keep warnings adjacent to the decision they qualify.
- Never hide fees, external outputs, network, or signing risk to save a page.

## Device matrix

- Jade 1.0: selection wheel.
- Jade 1.1: rocker/jog-wheel.
- Jade v2: 320x170 display with left/right selection buttons and camera.
- Jade v2c: Jade v2-family screen without camera/battery.
- DIY profiles: select-only, single button, navigation buttons, wheel, and
  touchscreen variations.

Use libjade GUI for rapid flow checks and QEMU web display for firmware-rendered
interaction. Compile actual affected targets because emulators do not prove
pixel geometry, touch calibration, power/backlight, or camera behavior.

