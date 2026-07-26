# USB mass-storage guidance

USB storage exposes a removable-media trust boundary.

- Treat filenames, directory entries, file sizes, and file contents as untrusted.
- Bound reads and writes, reject traversal/unsupported formats, and preserve
  explicit user confirmation before importing or signing data.
- Keep USB mode transitions, mount/unmount cleanup, and task ownership correct
  on all exits.
- Do not allow removable media to bypass the normal network, wallet,
  authentication, or signing policy checks.
- Compile Jade v2/ESP32-S3 and exercise malformed media/file cases where
  practical.

