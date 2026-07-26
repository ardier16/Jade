# Bootloader guidance

Bootloader changes affect the device root of trust and recovery path.

- Keep secure-boot, anti-rollback, partition, image verification, and recovery
  behavior fail closed.
- Do not change trust roots, signature thresholds, eFuse assumptions, partition
  offsets, or downgrade policy without explicit scope and security review.
- Never test by flashing physical hardware unless explicitly requested and a
  recovery method is confirmed.
- Validate production and development configurations separately and inspect
  bootloader size/partition output from `idf.py all`.

