# GitLab CI guidance

The root `.gitlab-ci.yml` defines stages and includes the jobs in this directory.

- Preserve the pinned builder/toolchain assumptions unless an upgrade is the
  task.
- Keep production, development, CI, radio-disabled, QEMU, libjade, sanitizer,
  release, and DIY jobs semantically distinct.
- Do not weaken format, config-diff, sanitizer, SAST, dependency, or secret
  detection checks to make a pipeline green.
- Keep release and flash jobs explicit/manual or runner-scoped. Do not add
  credentials, keys, device paths, or network secrets to YAML.
- Mirror local validation commands with CI and preserve artifact paths expected
  by downstream jobs.

Inspect the full include graph and `needs` relationships before changing a
shared template.

