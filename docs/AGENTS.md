# API documentation guidance

Sphinx sources here describe the public Jade RPC API.

- Keep method names, parameters, required/optional fields, result/error shapes,
  sequencing, authentication requirements, and firmware version notes aligned
  with handlers and `jadepy`.
- Document security-relevant user confirmation and network constraints.
- Do not document debug-only handlers as production API.
- Build with `make -C docs html` and inspect warnings and generated pages.

