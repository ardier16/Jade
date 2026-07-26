# Power abstraction guidance

Power implementations are selected per board and affect shutdown, battery,
backlight, and peripheral lifecycle.

- Preserve board-specific initialization/deinitialization ordering and release
  I2C/peripheral handles exactly once.
- Check error paths and behavior with absent/unsupported PMIC features.
- Avoid busy loops or changes that increase idle power unintentionally.
- Coordinate brightness and idle-timeout changes with `storage.c`,
  `idletimer.c`, and display code.
- Build every board family touched by a shared power change; emulator behavior
  is not sufficient evidence.

