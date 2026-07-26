# QEMU guidance

QEMU is an ESP32 development target used for automated firmware tests and an
optional web display. It is not a hardware-accurate Jade v2 substitute.

- Prefer `./main/qemu/run_emulator.sh` for an isolated Docker-backed run. It
  copies the repository so uncommitted work is included without replacing the
  active local configuration.
- TCP port `30121` carries Jade traffic; `30122` serves the web display.
- `tools/switch_to.sh qemu ...` deletes the active build/configuration, so use it
  directly only when that replacement is intentional.
- Keep QEMU stubs honest: do not make production firmware depend on behavior
  implemented only by the emulator.
- Test user interaction with `--webdisplay`; test protocol behavior through the
  TCP transport with `test_jade.py --qemu`.

Do not use QEMU success as evidence for secure storage, physical timing,
side-channel resistance, camera quality, BLE behavior, or exact device layout.

