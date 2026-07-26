# Hardware source guidance

This directory contains board design sources, schematics, Gerbers, and BOM data.

- Preserve source/export relationships and identify which file is authoritative
  for a change.
- Do not regenerate binary/proprietary CAD files without the matching tool and
  an explicit request.
- Review electrical, pin, PMIC, display, camera, USB, and secure-element/eFuse
  assumptions against firmware Kconfig and board code.
- Never present an unverified design edit as manufacturing-ready.

