# Signing flow map

## Entry points

| Flow | Python client | Firmware handler | Confirmation UI | Common fixtures |
|---|---|---|---|---|
| Bitcoin transaction | `JadeAPI.sign_tx` | `main/process/sign_tx.c` | `main/ui/sign_tx.c` | `tx_*`, `txn_*` |
| Liquid transaction | `JadeAPI.sign_liquid_tx` | `main/process/sign_tx.c` | `main/ui/sign_tx.c` | `liquid_txn_*`, `tx_liquid_*` |
| PSBT/PSET | `JadeAPI.sign_psbt` | `main/process/sign_psbt.c` | `main/ui/sign_tx.c` | `psbt_*`, `pset_*` |
| Message/file | `JadeAPI.sign_message*` | `main/process/sign_message.c` | `main/ui/sign_message.c` | `msg*` |
| Identity | `JadeAPI.sign_identity` | `main/process/sign_identity.c` | `main/ui/sign_identity.c` | `identity_*` |
| BIP85 digests | `JadeAPI.sign_bip85_digests` | `main/process/sign_bip85_digest.c` | feature-specific | RSA/vector cases |

`main/process/dashboard.c` authorizes and dispatches initial methods. It rejects
`tx_input`, `get_signature`, and `get_extended_data` when they arrive outside
the expected multi-message protocol.

## Shared implementation

- `main/process/sign_utils.*`: shared parameter validation and signing helpers.
- `main/process/process_utils.*`: network, descriptor, multisig, input, and
  extended-data parsing.
- `main/wallet.*`: derivation, scripts, policy, and transaction signing.
- `main/signer.*`: signer/multisig structures and validation.
- `main/assets.*` and `components/assets/`: Liquid asset metadata.
- `main/utils/psbt.*`, `main/descriptor.*`, `main/multisig.*`: wallet/input
  interpretation.
- `main/process.c`: request/reply IDs, sequencing, sources, and cleanup.
- `main/qrmode.c` and `main/usbhmsc/usbmode.c`: non-RPC entry paths that can
  invoke PSBT signing and must preserve the same policy/UI behavior.

## Security invariants

- Require the correct unlocked message source; do not let a second transport
  inherit authorization.
- Bind every continuation to the original request ID/method and expected
  sequence number.
- Validate network type against the loaded wallet restriction.
- Derive ownership and change status on-device from scripts/paths/registrations.
- Verify fees, amounts, assets, commitments, sighashes, and change summaries
  from transaction data rather than trusting host summaries.
- Display all external outputs and the final fee/warning before signing.
- Reject unsupported sighashes, paths, scripts, or mixed wallet policies.
- Keep anti-exfil host commitment/entropy sequencing intact and never reuse
  signer nonces.
- Clear input data, private keys, hashes, commitments, and temporary buffers on
  every exit.

## Test families

`test_jade.py` maps glob families to high-level signing paths. Useful focused
commands after building libjade include:

```sh
python test_jade.py --log CRITICAL --libjade --json-filter 'tx_ss_*'
python test_jade.py --log CRITICAL --libjade --json-filter 'psbt_*'
python test_jade.py --log CRITICAL --libjade --json-filter 'pset_*'
python test_jade.py --log CRITICAL --libjade --json-filter 'liquid_txn_*'
```

Use `--nolegacyflow` in a wider run to force the non-legacy/anti-exfil path.
Focused JSON runs skip unrelated low/high-level cases but still initialize the
wallet and registrations required by the vector families.

## Regression checklist

- Correct signature and expected number/order of signatures.
- A classification regression test proves that every external output reaches
  explicit confirmation; a correct returned signature alone does not prove the
  UI showed the output.
- User cancellation produces no signature and leaves the process idle.
- Malformed CBOR, truncated transaction, excessive counts, and wrong types fail.
- Wrong network/path/script/change classification fails.
- Already-signed or not-ours inputs are handled according to the contract.
- Bitcoin versus Liquid and PSBT versus PSET do not cross accepted paths.
- Legacy and anti-exfil behavior remain compatible.
- No memory-stat regression or sanitizer finding occurs.
