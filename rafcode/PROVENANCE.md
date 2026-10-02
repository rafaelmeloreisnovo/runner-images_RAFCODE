<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# Provenance and copyright boundary

## Repository inheritance

This repository contains substantial upstream material from GitHub's `actions/runner-images`. The root `LICENSE` currently identifies that upstream material under the MIT License and retains GitHub's copyright notice.

RAFCODE additions in `rafcode/` are intended as new, clean-room material. Their authorship does **not** replace, erase or absorb third-party authorship elsewhere in the repository.

## Per-file rule

New original RAFCODE text/source files use:

```text
SPDX-FileCopyrightText: 2026 Rafael Melo Reis
SPDX-License-Identifier: MIT
```

When a future file contains third-party material, record every applicable copyright holder and the actual license expression for that material. Do not relabel copied code as original.

## Firmware, ROM and manuals

Do not commit proprietary BIOS/ROM/EPROM images, vendor firmware, copyrighted manual scans, or copied source merely because they are useful for emulation. Prefer:

1. public specifications and datasheets with redistribution terms checked;
2. interface descriptions written independently;
3. hashes/metadata that identify a user-supplied artifact without redistributing it;
4. tiny factual constants only when licensing/quotation rules allow them;
5. independently written tests against abstract behavior.

## Evidence classes

- `SOURCE` — upstream specification, source file, datasheet, commit or independently supplied artifact.
- `ARTIFACT` — RAFCODE output derived from an authorized source.
- `EXECUTION` — a concrete compiler/emulator/hardware run.
- `EVIDENCE` — retained output that can be independently inspected.
- `CLAIM` — conclusion permitted by the evidence gate.

`SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`.

## Standards posture

SPDX/REUSE conventions are used for machine-readable copyright/license metadata. NIST SSDF concepts may be used as secure-development guidance. This project must not describe itself as certified, audited, conformant or compliant with a standard unless the corresponding scope, evidence and assessment actually exist.
