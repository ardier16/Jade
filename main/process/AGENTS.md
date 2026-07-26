# RPC process guidance

Each handler processes untrusted CBOR from a host or an internal source.

## Handler workflow

1. Confirm the expected method, device state, authentication state, and message
   source.
2. Parse required and optional parameters with the existing helpers. Reject
   unknown or inconsistent data where the protocol requires it.
3. Validate network, derivation path, transaction structure, sizes, and counts
   before allocation or cryptographic work.
4. Register cleanup as soon as ownership transfers to the process.
5. Display all security-relevant facts and require the established user action
   before using private material.
6. Send exactly one result or structured rejection and preserve request IDs.

Keep parsing and shared validation in `process_utils.*` or `sign_utils.*` only
when multiple handlers genuinely share it. Maintain compatibility with
`jadepy/jade.py` and both legacy and anti-exfil signing flows.

For signing changes, read `.codex/skills/jade-signing-engineer/SKILL.md`. Add
positive and negative vectors under `test_data/` and run focused libjade tests
before the wider suite.

Never accept host-provided change classification, fee data, asset metadata, or
signing summaries without independently verifying the fields the firmware can
derive.

