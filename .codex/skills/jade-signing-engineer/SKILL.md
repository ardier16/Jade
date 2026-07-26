---
name: jade-signing-engineer
description: Implement, debug, or review Jade transaction, PSBT/PSET, message, identity, and anti-exfil signing flows across firmware handlers, wallet/signing utilities, confirmation UI, jadepy, and test vectors. Use for signing failures, new transaction types, signing-page behavior, signature sequencing, change/output validation, Bitcoin or Liquid policy, and tests matching tx_*, psbt_*, pset_*, or liquid_txn_* fixtures.
---

# Jade Signing Engineer

Treat signing as one security protocol spanning untrusted host input, device
policy, user verification, private-key use, and response sequencing.

## Prepare

Read the root, `main/process/`, and `main/ui/` agent guidance plus
[references/signing-flow.md](references/signing-flow.md). Use
`jade-client-protocol` for public RPC changes, `jade-ui-engineer` for new screen
designs, and `jade-security-reviewer` for every signing-policy change.

## Workflow

1. Identify the exact flow: Bitcoin transaction, Liquid transaction, PSBT/PSET,
   message/file, identity, BIP85, attestation, legacy signatures, or anti-exfil.
2. Reproduce the failure with the narrowest existing fixture before editing.
3. Trace the request from `jadepy` through RPC dispatch, parameter parsing,
   wallet/signing helpers, user confirmation, signature generation, and reply.
4. Separate host claims from device-derived facts. Independently validate
   networks, paths, scripts, inputs, change, amounts/assets, fees, sighashes,
   commitments, and anti-exfil data as applicable.
5. Make the confirmation UI show the complete facts the user must authorize.
   Preserve cancellation and never let CI defaults leak into production.
6. Register cleanup immediately for allocations or secret state and clear every
   path, including parse, user rejection, and crypto failures.
7. Preserve request IDs, multi-message ordering, and legacy compatibility unless
   a breaking protocol change is explicitly required.
8. Add positive, malformed, policy-rejection, and regression vectors.

## Validate

- Run the matching `--json-filter` family against libjade.
- Exercise both legacy and anti-exfil flows when the feature supports both.
- Cover Bitcoin/Liquid and PSBT/PSET distinctions affected by the change.
- Run the full libjade suite for shared signing utilities; use sanitizer builds
  for memory/ownership changes.
- Inspect interactive confirmation with libjade GUI or QEMU, then compile the
  preserved Jade v2 target.

Do not fix a signing test by weakening validation or merely replacing expected
fixture output. Establish which behavior is correct first.

