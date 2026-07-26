# Logo and binary artwork guidance

Compressed `.bin.gz` files are firmware assets derived from image data.

- Do not hand-edit compressed binaries.
- Preserve dimensions, BGR565 byte order, status-bar size selection, and
  regulatory artwork fidelity.
- Record or retain the source conversion path and regenerate deterministically
  using the scripts/commands documented here.
- Build affected display sizes and visually inspect the result in QEMU/libjade
  when possible.

