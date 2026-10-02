<!-- SPDX-FileCopyrightText: 2026 Rafael Melo Reis -->
<!-- SPDX-License-Identifier: MIT -->

# RAFCODE semantic seams

The purpose of this inventory is to keep the multidimensional meaning where disciplines touch, without copying a historical software architecture.

Each seam must answer five questions:

`what crosses? | representation? | bound? | authority? | evidence?`

## 1. Materials ↔ semiconductor device

What crosses: conductivity, junction behavior, dielectric/magnetic/thermal constraints.

Software representation: metadata/model input only.

Gate: no electrical claim from software compilation.

## 2. Device ↔ digital logic

What crosses: thresholded state transition and timing constraints.

Software representation: discrete state plus explicit unknown/error state where required.

Gate: analog uncertainty cannot silently become Boolean certainty.

## 3. Logic ↔ timing

What crosses: setup/hold/order/propagation requirements.

Software representation: boundary metadata or measured timing lane.

Gate: lower latency is not security if validation was removed.

## 4. Logic ↔ memory

What crosses: state retention, mutability, persistence and addressability.

Software representation: bounded static state; no hidden heap in the generic core.

Gate: duplicated shadow state requires an explicit authority rule or is rejected.

## 5. Memory ↔ interconnect

What crosses: address, payload relation, ownership/order and completion.

Software representation: normalized event plus address map.

Gate: transport ordering is not assumed globally.

## 6. Interconnect ↔ overlap core

What crosses: `{kind, source, target, code, relation, value}`.

Software representation: deterministic locus in a fixed overlap field.

Gate: no implicit FIFO, no hidden retry list, no unbounded accumulation.

## 7. CRC/integrity vocabulary ↔ overlap core

What crosses: compact relation token.

Software representation: `relation` field.

Gate: CRC equality does not establish authenticity, provenance or trust. A CRC may influence composition/routing observation without being a security verdict.

## 8. Addressing ↔ legacy compatibility

What crosses: legacy address window and selected target window.

Software representation: checked base/span translation.

Gate: overflow, alias and out-of-range mappings fail explicitly.

## 9. Firmware ↔ boot

What crosses: reset/entry contract, discovery result, selected boot object.

Software representation: interface/state model.

Gate: unknown firmware provenance remains `TOKEN_VAZIO`; proprietary payloads are not copied into the core.

## 10. Boot ↔ storage/filesystem

What crosses: bounded block/file structure required to locate the next object.

Software representation: isolated compatibility adapter.

Gate: MBR, FAT, NTFS, ext and similar formats do not dictate the internal core architecture.

## 11. Runtime ↔ observability

What crosses: state transition, timing, counters and evidence identifiers.

Software representation: append-only evidence outside the generic state core.

Gate: observability must not create a second authoritative runtime state.

## 12. Security ↔ latency

What crosses: deviation from a measured/declared execution envelope.

Software representation: anomaly signal only.

Gate: latency is evidence about behavior, not proof of safety. A timing anomaly may trigger investigation/fail-closed policy, but normal timing cannot prove absence of exploitation.

## 13. Legacy knowledge ↔ clean-room implementation

What crosses: required behavior and semantic obligation only.

Software representation: project-native contract written independently.

Gate: source bodies, condition trees, skeletons and implementation-specific constants do not cross this seam.

## 14. Authorial expression ↔ novelty claim

What crosses: independently written artifact plus prior-art evidence.

Software representation: separate provenance and novelty ledgers.

Gate: `AUTHORIAL_EXPRESSION` may be evidenced while `NOVELTY` remains `TOKEN_VAZIO`.

## Current highest-value seam

The present correction targets seam 6 and seam 7: replacing queue-based core semantics with bounded overlap composition and separating CRC-derived relation from trust/verification semantics.
