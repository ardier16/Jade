# DIY hardware documentation guidance

DIY targets are community development configurations, not official Jade
production units.

- Keep wiring, board profile, ESP32/ESP32-S3 target, camera/display/input, baud,
  and secure-boot caveats aligned with `configs/`.
- Clearly separate build/flash instructions for DIY or development devices from
  supported firmware update instructions for official hardware.
- Do not claim equivalent physical security, enclosure security, entropy, or
  certification.
- Validate links and build the referenced config when configuration commands
  change.

