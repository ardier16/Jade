# Test vector guidance

Files here are protocol, transaction, PSBT/PSET, descriptor, multisig, message,
identity, OTP, and QR fixtures consumed by `test_jade.py`.

- Treat an existing expected result as a specification clue, not an obstacle to
  overwrite. Confirm the behavior in firmware/client code before changing it.
- Add the smallest positive vector and relevant negative vectors for parsing,
  network, amount/asset, change, path, signature, and policy failures.
- Follow existing filename prefixes because `--json-filter` and test helpers use
  glob families such as `tx_*`, `psbt_*`, `pset_*`, `liquid_txn_*`, and
  `descriptor_*`.
- Keep binary `.dat` and JSON pairs synchronized when both represent one case.
- Test mnemonics and keys are deterministic fixtures only. Do not reuse them
  outside tests.
- Do not bulk-reformat large JSON fixtures without a functional reason.

Validate new vectors with:

```sh
python test_jade.py --log CRITICAL --libjade --json-filter 'matching_glob'
```

