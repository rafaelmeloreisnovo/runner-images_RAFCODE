<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# RAFCODE semantic seams

Each seam records `what crosses | representation | bound | authority | evidence`.

- materials ↔ device: physical/electrical constraints; software cannot claim electrical PASS.
- device ↔ logic: thresholded state plus explicit unknown/error where needed.
- logic ↔ timing: timing is evidence, not a substitute for validation.
- logic ↔ memory: fixed bounded state; duplicated shadow state needs an authority rule.
- memory ↔ interconnect: address, payload relation, ownership/order and completion.
- interconnect ↔ overlap core: normalized event enters a deterministic bounded locus; no hidden FIFO.
- CRC/integrity vocabulary ↔ overlap: compact relation token only; CRC equality is not trust.
- addressing ↔ legacy compatibility: checked translation; overflow/alias/out-of-range are explicit failures.
- firmware ↔ boot: interface/state model; unknown provenance remains TOKEN_VAZIO.
- boot ↔ storage/filesystem: compatibility adapter; historical formats do not dictate core architecture.
- runtime ↔ observability: evidence must not become a second authoritative runtime state.
- security ↔ latency: timing deviation can be an anomaly signal; normal latency cannot prove safety.
- legacy knowledge ↔ clean-room implementation: behavior obligations may cross; source bodies/skeletons do not.
- authorial expression ↔ novelty: independent code can be evidenced while novelty remains TOKEN_VAZIO.

The current correction concentrates on the interconnect/overlap and CRC/overlap seams.
