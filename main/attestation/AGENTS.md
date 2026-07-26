# Attestation guidance

Attestation binds challenges and device identity to protected key material.

- Preserve key provenance, challenge length checks, signature format, and
  production/development separation.
- Never expose attestation private material or add fallback keys.
- Reject malformed or replay-prone input before signing.
- Keep registration and signing RPC behavior synchronized with `jadepy` and
  `test_jade.py`.
- Treat changes as security-sensitive and use the `jade-security-reviewer`
  checklist plus focused positive and negative tests.

