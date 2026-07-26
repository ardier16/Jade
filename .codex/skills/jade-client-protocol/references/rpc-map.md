# RPC and client map

## Message lifecycle

1. A transport pushes CBOR into the firmware message queue in `main/process.c`.
2. `main/process/dashboard.c` checks device/authentication state and dispatches
   the initial method.
3. A handler in `main/process/` parses parameters and may run a multi-message
   protocol.
4. `main/process.c` emits a result or common structured error with the request
   ID on the correct source.
5. `jadepy/jade.py::_jadeRpc` drives the corresponding client exchange over a
   transport implementation.

## Public surfaces

- Firmware common CBOR/error helpers: `main/utils/cbor_rpc.*`.
- Handler parsing helpers: `main/process/process_utils.*`.
- Public Python API: `jadepy/jade.py`.
- Transports: `jade_serial.py`, `jade_ble.py`, `jade_tcp.py`, `jade_sw.py`.
- Error type: `jadepy/jade_error.py`.
- Protocol documentation: `docs/index.rst`.
- Integration/negative tests: `test_jade.py`.

## Contract checklist

- Method availability before/after initialization and unlock.
- Binding to the message source that authorized the wallet.
- Request ID length/echo and out-of-order behavior for nonblocking `ping`.
- Required/optional parameter types and explicit bounds.
- Network names and loaded-wallet network restrictions.
- Result type/shape and common error code/message/data.
- Physical confirmation delay and long timeout requirements.
- Continuation method, original ID/method, sequence number/length, and terminal
  response for multi-message calls.
- Firmware/client compatibility when one side lacks the new behavior.

## Multi-message examples

- Legacy transaction signing streams `tx_input` messages and signatures.
- Anti-exfil signing uses commitments followed by `get_signature`.
- Large PSBT replies use `get_extended_data` with original request binding and
  sequential fragments.
- PIN authentication may request an HTTP callback through the client.

Continuation methods are rejected by dashboard dispatch when they arrive
outside their owning handler's active protocol.

## Client rules

- Keep `_jadeRpc` as the common exchange layer.
- Do not make a public method depend on one transport without documenting and
  enforcing that limitation.
- Keep optional BLE and HTTP dependencies optional at import time.
- Validate reply structure before concatenating fragments or returning values.
- Avoid logging raw CBOR or hexified secrets.
- Retain context-manager connect/disconnect behavior and transport timeouts.

## Documentation and tests

Document public methods in `docs/index.rst` with request/reply CBOR examples,
state/user-interaction notes, and errors. Update Python docstrings as needed.
Test happy path, missing/wrong fields, invalid state, unknown/unexpected method,
sequence mismatch, wrong network, user rejection, and transport-independent
behavior through libjade.

