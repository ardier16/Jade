# libjade guidance

libjade is an insecure, development-only Linux-native runtime for firmware
logic and UI testing. External callers can inspect storage, screens, input, and
camera state.

- Keep stubs behaviorally close enough to firmware to expose bugs; do not make
  shared firmware depend on libjade-only behavior.
- Check both in-process `JadeAPI.create_libjade()` and daemon/serial-style paths
  when changing the public native interface.
- Preserve thread, event, task, and ownership semantics used by firmware.
- Do not claim that libjade verifies secure storage, timing, memory wiping,
  paging resistance, hardware entropy, or side-channel properties.
- Add new ESP-IDF compatibility stubs only when required by shared firmware and
  document unsupported semantics in the closest existing source/header.

Default checks in a supported Linux environment or the repository builder:

```sh
./libjade/make_libjade.sh Debug
export LD_LIBRARY_PATH="$PWD/build_linux/libjade${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
python test_jade.py --log CRITICAL --libjade
./libjade/make_libjade.sh Sanitize
```

Use `./libjade/run_libjade_gui.sh Debug` for interactive UI work. The loader
currently expects `libjade.so` and Linux `LD_LIBRARY_PATH`; on macOS use the
Linux builder or QEMU instead of treating a native `.dylib` as supported.
Sanitizer runtime setup is platform-specific; follow the command printed by the
build or the GitLab job.
