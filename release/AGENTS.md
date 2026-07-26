# Release guidance

Release scripts are high-impact even when they use development keys.

- Default to inspection, static validation, and dry-run reasoning.
- Do not invoke signing, apply signatures, stage a release, promote beta,
  publish/upload artifacts, or contact firmware servers without an explicit
  request.
- Before an authorized operation, identify the exact script, input artifacts,
  key class, expected output directory, remote side effects, and recovery path.
  Ask for final confirmation for production key operations.
- Never print private key contents. Do not replace keys, trust roots, signature
  thresholds, version constraints, or artifact hashes as incidental cleanup.
- Preserve deterministic artifact naming and multi-signature ordering.
- Validate changes with fixtures or development artifacts, never production
  artifacts, unless the user explicitly provides and authorizes them.

Changes here require the `jade-build-config` and `jade-security-reviewer`
checklists.

