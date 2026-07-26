# Firmware utility guidance

Utilities here are shared by many security-sensitive handlers.

- Keep parsing helpers strict and side-effect free where possible.
- Use size-aware APIs and checked arithmetic for CBOR, PSBT, address, URL, and
  temporary-stack operations.
- Preserve Bitcoin versus Liquid and main/test network distinctions.
- Do not broaden acceptance rules to fix one caller without evaluating every
  caller.
- Add low-level or vector coverage for edge cases and build both amalgamated and
  unamalgamated paths when declarations/source inclusion change.

