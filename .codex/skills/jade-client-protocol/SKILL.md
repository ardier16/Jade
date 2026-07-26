---
name: jade-client-protocol
description: Add, modify, debug, or document Jade CBOR RPCs and the public Python jadepy client across firmware dispatch/handlers, multi-message sequencing, transports, error shapes, compatibility, Sphinx API docs, and test_jade.py. Use for new device methods, changed parameters/results, Python API methods, serial/BLE/TCP/libjade differences, request IDs, chunking, HTTP callbacks, or protocol version behavior.
---

# Jade Client Protocol

Keep the firmware contract and every client transport synchronized.

## Prepare

Read the root, `main/process/`, `jadepy/`, and `docs/` guidance. Read
[references/rpc-map.md](references/rpc-map.md). Add `jade-signing-engineer` for
signing methods and `jade-security-reviewer` for authentication, key export,
update, or trust-policy changes.

## Workflow

1. Write the method contract: availability/state, parameters, optional fields,
   bounds, result, structured errors, sequencing, timeouts, and compatibility.
2. Implement strict firmware parsing and one terminal reply/rejection per
   request ID.
3. Add or update the high-level method in `jadepy/jade.py` without leaking
   transport details into the public contract.
4. Ensure serial, BLE, TCP, and libjade transports can carry the same exchange;
   handle chunking and callbacks through existing `_jadeRpc` mechanisms.
5. Preserve old clients/firmware behavior where possible. Gate genuinely new
   behavior using established version/capability patterns.
6. Add happy-path and malformed/incorrect-state coverage.
7. Update `docs/index.rst` for public protocol changes and Python docstrings for
   public client changes.

## Validate

Run syntax/style checks, focused libjade integration tests, negative protocol
cases, and the full suite for shared RPC machinery. Build the Python package for
packaging metadata changes and compile Jade v2 for firmware changes.

