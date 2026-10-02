<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# Attack-surface reduction matrix

This file records defensive relationships only. It does not contain exploit procedures or payloads.

| Surface | Exposed condition | Relationship to known vulnerability class | Reduction state |
|---|---|---|---|
| event ring | mutable head/tail/count accepted without invariant check | corrupted state -> out-of-bounds memory access class | CLOSED_IN_CORE |
| event slot | data remained after logical pop | residual-data / stale-state disclosure class | CLOSED_IN_CORE |
| event/map flags | undefined public fields | hidden capability / ambiguous parser-dispatch class | REMOVED |
| address map | span could describe an interval wrapping the address space | integer-wrap / range-confusion class | CLOSED_IN_CORE |
| event kind | zero or unknown kind could enter queue | ambiguous dispatch / fail-open class | CLOSED_IN_CORE |
| assembly objects | stack executable marking left to toolchain defaults | executable-stack hardening gap | REDUCED |
| CI action dependency | mutable action tag | CI/CD supply-chain dependency substitution class | PINNED_SHA |
| checkout credentials | checkout could persist token material in local git config | post-checkout credential reuse/exfiltration class | PERSISTENCE_DISABLED |
| CI runner label | `ubuntu-latest` changes under the same workflow text | non-deterministic environment / moving attack surface | FIXED_FAMILY_24_04 |
| inherited repository-dispatch workflows | event payload directly reaches shell/script; force-push/approval paths exist | workflow injection / over-privileged automation class | OPEN_REQUIRES_SEPARATE_PATCH |
| main branch | branch reported unprotected by current repository readback | governance / unauthorized-change blast-radius class | OPEN_REQUIRES_POLICY |
| unknown 0-day | not observable by source inspection | unknown vulnerability | TOKEN_VAZIO |

## Gate rule

`KNOWN_CLASS_REDUCED` does not imply `ZERO_DAY_ABSENT`.

A gate may become `PASS` only for a property that has an executable or inspectable test. Unknown vulnerabilities remain `TOKEN_VAZIO`.
