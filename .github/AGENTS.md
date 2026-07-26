# GitHub Actions guidance

GitHub Actions provides a smaller public build matrix than GitLab CI. Keep the
ESP-IDF version and target commands aligned with supported repository builds.

- Preserve recursive submodule checkout.
- Do not treat the GitHub matrix as a replacement for GitLab's libjade,
  sanitizer, config, release, or hardware jobs.
- Pin third-party actions to an intentional version and review permission or
  credential changes carefully.
- Do not add physical flashing, signing, publishing, or production secrets
  without an explicit request and a separately reviewed security design.

