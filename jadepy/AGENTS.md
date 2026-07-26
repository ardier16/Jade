# Python client guidance

`jade.py` is the public high-level API. The other modules implement transport
details and should not duplicate protocol policy.

- Preserve public method signatures and return shapes unless the task explicitly
  accepts a breaking change.
- Keep firmware RPC names, parameter keys, sequence IDs, chunking, and error
  handling synchronized with `main/process.c` and `main/process/`.
- Support transport-independent behavior through `_jadeRpc`; do not special-case
  serial, BLE, TCP, or libjade unless the transport requires it.
- Treat all device replies as untrusted: validate expected types/shapes before
  combining multi-message responses.
- Never log secrets in normal or debug output. `_hexlify` makes byte values easy
  to leak, so inspect new logging carefully.
- Keep BLE and HTTP dependencies optional.

Validate syntax/style, build the package with `python -m build` when packaging
changes, and run affected high-level cases in `test_jade.py` against libjade.
Protocol changes also require firmware negative tests and public documentation
updates where relevant.

